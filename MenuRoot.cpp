#include "MenuRoot.hpp"
#include "Logger.hpp"
#include <format>
#include "List.hpp"
#include "MenuInput.hpp"
#include "Slider.hpp"

void MenuRoot::DrawRoot()
{
	PencilX = X;
	PencilY = Y;

	GUI.DrawFilledRect(PencilX, PencilY, MenuWidth, RootHeight, ActiveColour);
	GUI.AddText(GetAddressText().c_str(), PencilX, PencilY + (RootPadding / 2), {1, 1, 1, 1}, ADDRESS_TEXT_SIZE);

	// display command index / count on the right
	if (DoesSelectedTabIndexExist())
	{
		int CommandCount = Tabs[SelectedTabIndex]->Commands.size();
		if (!Tabs[SelectedTabIndex]->ListChain.empty())
		{
			auto& CurrentList = Tabs[SelectedTabIndex]->ListChain.back();
			CommandCount = CurrentList->Commands.size();
		}

		if (CommandCount != 0)
		{
			std::string CommandCountText = std::format(
				"{} / {}", SelectedCommandIndex + 1, CommandCount
			);
			float TextWidth = GUI.GetTextWidth(CommandCountText.c_str(), ADDRESS_TEXT_SIZE);
			GUI.AddText(CommandCountText.c_str(), PencilX + (MenuWidth - TextWidth - 8), PencilY + (RootPadding / 2), { 1, 1, 1, 1 }, ADDRESS_TEXT_SIZE);
		}
	}

	PencilY += RootHeight + RootPadding;

	if (Tabs.empty() || !Menu.DoesSelectedTabIndexExist())
		return;

	// Tabs
	for (size_t Index = 0; Index < Tabs.size(); ++Index)
	{
		auto& CurrentTab = Tabs[Index];
		bool Active = (Index == SelectedTabIndex);

		CurrentTab->Draw(GUI, PencilX, PencilY, Active);

		float RemainingWidth = MenuWidth - CurrentTab->Width - RootPadding;

		if (CurrentTab->Commands.empty())
			continue;

		if (Active)
		{ // draw commands
			float OldPencilY = PencilY;
			float OldPencilX = PencilX;

			std::vector<std::shared_ptr<Command>> CommandsToDraw;

			PencilX += CurrentTab->Width + RootPadding; // move pencil to the right of the tabs
			PencilY = Y + RootHeight + RootPadding; // reset pencil Y to below root

			if (!CurrentTab->ListChain.empty()) // draw from the last opened list instead
			{
				auto& CurrentList = CurrentTab->ListChain.back();
				
				for (size_t Index = 0; Index < CurrentList->Commands.size(); ++Index)
				{
					auto& CurrentCommand = CurrentList->Commands[Index];
					if (!CurrentCommand)
						continue;
					CommandsToDraw.push_back(CurrentCommand);
				}
			}
			else // draw commands from the tab
			{
				for (size_t Index = 0; Index < CurrentTab->Commands.size(); ++Index)
				{
					auto& CurrentCommand = CurrentTab->Commands[Index];
					if (!CurrentCommand)
						continue;
					CommandsToDraw.push_back(CurrentCommand);
				}
			}

			// scrolling
			int Total = CommandsToDraw.size();
			int StartIndex = VisibleStartIndex;
			int EndIndex = Total;

			if (Total > MaxVisibleCommands)
			{
				SelectedCommandIndex = std::clamp(SelectedCommandIndex, 0, Total - 1);

				int MaxStart = Total - MaxVisibleCommands;
				bool MovingDown = SelectedCommandIndex > PreviousSelectedCommandIndex;
				bool MovingUp = SelectedCommandIndex < PreviousSelectedCommandIndex;

				if (MovingDown &&
					SelectedCommandIndex >= StartIndex + (MaxVisibleCommands - MoveDownAt))
				{
					StartIndex = SelectedCommandIndex - (MaxVisibleCommands - MoveDownAt);
				}

				if (MovingUp &&
					SelectedCommandIndex <= StartIndex + MoveDownAt)
				{
					StartIndex = SelectedCommandIndex - MoveDownAt;
				}

				StartIndex = std::clamp(StartIndex, 0, MaxStart);
				EndIndex = StartIndex + MaxVisibleCommands;
				VisibleStartIndex = StartIndex;
			}

			for (size_t Index = StartIndex; Index < EndIndex; ++Index)
			{
				auto& CurrentCommand = CommandsToDraw[Index];
				bool Active = (Index == SelectedCommandIndex);
				CurrentCommand->Draw(GUI, PencilX, PencilY, RemainingWidth, Active);
			}

			// prepare for next tab
			PencilX = OldPencilX;
			PencilY = OldPencilY;
		}
	}

	PreviousSelectedCommandIndex = SelectedCommandIndex;
}

void MenuRoot::HandleInput(InputHandler& Input)
{
	if (DoesSelectedTabIndexExist())
	{ // Command list input

		if (Input.Hotkeys["Up"].WasReleased() || Input.Hotkeys["Up"].IsRepeating())
		{
			SelectedCommandIndex -= 1;

			if (DoesSelectedCommandIndexExist() && GetSelectedCommand()->GetType() == CT_Divider)
				SelectedCommandIndex -= 1; // skip dividers

			if (SelectedCommandIndex < 0)
			{
				if (!Tabs[SelectedTabIndex]->ListChain.empty())
					SelectedCommandIndex = Tabs[SelectedTabIndex]->ListChain.back()->Commands.size() - 1;
				else
					SelectedCommandIndex = Tabs[SelectedTabIndex]->Commands.size() - 1;
			}
			if (DoesSelectedCommandIndexExist() == false)
				SelectedCommandIndex = 0;
		}

		if (Input.Hotkeys["Down"].WasReleased() || Input.Hotkeys["Down"].IsRepeating())
		{
			SelectedCommandIndex += 1;

			if (DoesSelectedCommandIndexExist() && GetSelectedCommand()->GetType() == CT_Divider)
				SelectedCommandIndex += 1; // skip dividers

			if (DoesSelectedCommandIndexExist() == false)
				SelectedCommandIndex = 0;
		}

		bool SelectReleased = Input.Hotkeys["Select"].WasReleased();
		if (SelectReleased)
		{
			// check if current command is a slider and adjust instead
			if (DoesSelectedCommandIndexExist())
			{
				auto CurrentCommand = GetSelectedCommand();
				if (CurrentCommand && CurrentCommand->GetType() == CT_Slider)
				{
					auto SliderCommand = std::dynamic_pointer_cast<Slider>(CurrentCommand);
					if (SliderCommand)
					{
						SliderCommand->OnRightAdjust();
						return;
					}
				}
			}
		}

		if (SelectReleased || Input.Hotkeys["Select_Secondary"].WasReleased())
		{
			if (DoesSelectedCommandIndexExist())
			{
				auto& CurrentTab = Tabs[SelectedTabIndex];
				if (!CurrentTab->ListChain.empty())
				{
					auto& CurrentList = CurrentTab->ListChain.back();
					auto& Command = CurrentList->Commands[SelectedCommandIndex];

					Command->OnClick();
				}
				else
				{
					auto& Command = CurrentTab->Commands[SelectedCommandIndex];
					Command->OnClick();
				}
			}
		}

		bool BackReleased = Input.Hotkeys["Back"].WasReleased();
		if (BackReleased)
		{
			// check if current command is a slider and adjust instead
			if (DoesSelectedCommandIndexExist() && !Input.Hotkeys["Back_Secondary"].WasReleased())
			{
				auto CurrentCommand = GetSelectedCommand();
				if (CurrentCommand && CurrentCommand->GetType() == CT_Slider)
				{
					auto SliderCommand = std::dynamic_pointer_cast<Slider>(CurrentCommand);
					if (SliderCommand)
					{
						SliderCommand->OnLeftAdjust();
						return;
					}
				}
			}
		}

		if (BackReleased || Input.Hotkeys["Back_Secondary"].WasReleased())
		{
			auto& CurrentTab = Tabs[SelectedTabIndex];
			if (!CurrentTab->ListChain.empty())
			{
				auto CurrentListName = CurrentTab->ListChain.back()->Name;
				CurrentTab->ListChain.pop_back();
		
				if (CurrentTab->ListChain.size() == 1) // select this list in the parent list
				{
					for (size_t Index = 0; Index < CurrentTab->ListChain.back()->Commands.size(); ++Index)
					{
						auto& Command = CurrentTab->ListChain.back();
						if (Command->Name == CurrentListName)
						{
							SelectedCommandIndex = Index;
							break;
						}
					}
				}
				else // select this list in the tab commands
				{
					for (size_t Index = 0; Index < CurrentTab->Commands.size(); ++Index)
					{
						auto& Command = CurrentTab->Commands[Index];
						if (Command->Name == CurrentListName)
						{
							SelectedCommandIndex = Index;
							break;
						}
					}
				}
			}
		}

		// Tab Navigation
		if (Input.Hotkeys["TabUp"].WasReleased())
		{
			SelectedTabIndex -= 1;
			if (SelectedTabIndex < 0)
				SelectedTabIndex = Tabs.size() - 1;
			SelectedCommandIndex = 0; // reset command selection
		}

		if (Input.Hotkeys["TabDown"].WasReleased())
		{
			SelectedTabIndex += 1;
			if (SelectedTabIndex >= Tabs.size())
				SelectedTabIndex = 0;
			SelectedCommandIndex = 0; // reset command selection
		}
	}
}

std::string MenuRoot::GetAddressText()
{
	auto& CurrentTab = Tabs[SelectedTabIndex];
	std::string Title = std::string(MenuTitle) + AddressDenominator + CurrentTab->Name;

	if (!CurrentTab->ListChain.empty())
	{
		for (const auto& List : CurrentTab->ListChain)
		{
			Title += AddressDenominator + List->Name;
		}
	}

	return Title;
}

bool MenuRoot::DoesSelectedCommandIndexExist()
{
	auto& CurrentTab = Tabs[SelectedTabIndex];
	if (!CurrentTab->ListChain.empty())
	{
		auto& CurrentList = CurrentTab->ListChain.back();
		return !CurrentList->Commands.empty() && SelectedCommandIndex >= 0 && SelectedCommandIndex < CurrentList->Commands.size();
	}

	return !Tabs[SelectedTabIndex]->Commands.empty() && SelectedCommandIndex >= 0 && SelectedCommandIndex < Tabs[SelectedTabIndex]->Commands.size();
}

std::shared_ptr<Command> MenuRoot::GetSelectedCommand()
{
	if (!DoesSelectedTabIndexExist() || !DoesSelectedCommandIndexExist())
		return nullptr;
	auto& CurrentTab = Tabs[SelectedTabIndex];
	if (!CurrentTab->ListChain.empty())
	{
		auto& CurrentList = CurrentTab->ListChain.back();
		return CurrentList->Commands[SelectedCommandIndex];
	}
	return Tabs[SelectedTabIndex]->Commands[SelectedCommandIndex];
}

void Tab::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, Width, Height, Hovered ? Menu.ActiveColour : Menu.NormalColour);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	PencilY += Height;
}

void Command::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, RemainingWidth, Height, Hovered ? Menu.ActiveColour : Menu.NormalColour);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), {1, 1, 1, 1}, COMMAND_TEXT_SIZE);
	PencilY += Height;
}

MenuRoot Menu;