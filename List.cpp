#include "List.hpp"

void List::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, RemainingWidth, Height, Hovered ? Menu.ActiveColour : Menu.NormalColour);
	GUI.AddTexture("List", PencilX + RemainingWidth - (Height * 0.75f), PencilY, { 1, 1, 1, 1 }, 0.13f);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	PencilY += Height;
}

void List::OnClick()
{
	if (Menu.DoesSelectedTabIndexExist())
	{
		std::shared_ptr<List> Self = std::make_shared<List>(*this);

		Menu.Tabs[Menu.SelectedTabIndex]->ListChain.push_back(Self);
	}
}