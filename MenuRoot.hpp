#pragma once
#include "GUICommon.hpp"
#include <vector>
#include <functional>

#define MenuName "Stand"
#define MenuVersion "25.12.1"

#ifndef NDEBUG
#define MenuTitle MenuName " " MenuVersion "-devdbg"
#else
#define MenuTitle MenuName " " MenuVersion
#endif

#define TEXT_HEIGHT_PX 0.021875
#define ADDRESS_TEXT_SIZE (12.0 * TEXT_HEIGHT_PX * 2.0)
#define COMMAND_TEXT_SIZE 0.65625f

enum CommandType
{
	CT_Command,
	CT_List,
	CT_Toggle,
	CT_Slider,
};

const DirectX::XMVECTORF32 NormalColour = { 0.0f, 0.0f, 0.0f, 0.3019f };
const DirectX::XMVECTORF32 ActiveColour = { 1, 0, 1, 1 };

/*
	A command is a feature.
	It can be a button, checkbox, slider.
	Commands look similar to tabs but fill up the entire right side of the menu.
*/
class Command
{
public:
	const float Height = 32.0f;

	std::string Name;
	std::vector<std::string> CommandNames; // short names that can be used for searching

	virtual void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, float Width, bool Hovered);

	// Interactions
	std::function<void()> OnClickCallback;
	virtual void OnClick()
	{
		if (OnClickCallback)
			OnClickCallback();
	}

	// Type identification
	virtual CommandType GetType() { return CT_Command; }

	Command(std::string InName)
		: Name(InName) {}
};

/*
	A tab shows below and on the left of the menu root.
	There is no padding between tabs.
*/
class Tab
{
private:
	
public:
	const float Width = 110.0f;
	const float Height = 32.0f;

	std::string Name;

	std::vector<std::shared_ptr<Command>> Commands;
	std::vector<std::shared_ptr<class List>> ListChain; // Chain of opened lists, so we can go back

	void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, bool Hovered);
	
	template<typename T>
	std::shared_ptr<T> Add(std::unique_ptr<T> InCommand)
	{
		Commands.push_back(std::move(InCommand));
		return std::dynamic_pointer_cast<T>(Commands.back());
	}

	template<typename T>
	std::shared_ptr<T> GetCommandByName(const std::string& InName)
	{
		for (auto& Command : Commands)
		{
			if (Command->GetType() == CT_List)
			{
				auto SubList = std::dynamic_pointer_cast<List>(Command);
				auto SubCommand = SubList->GetCommandByName<T>(InName);
				if (SubCommand)
					return SubCommand;
			}

			if (Command->Name == InName)
				return std::dynamic_pointer_cast<T>(Command);
		}
		return nullptr;
	}

	template<typename T>
	std::shared_ptr<T> GetCommandByCommandName(const std::string& InCommandName)
	{
		for (auto& Command : Commands)
		{
			if (Command->GetType() == CT_List)
			{
				auto& SubList = std::dynamic_pointer_cast<List>(Command);
				auto SubCommand = SubList->GetCommandByCommandName<T>(InCommandName);
				if (SubCommand)
					return SubCommand;
			}

			for (const auto& CommandName : Command->CommandNames)
			{
				if (CommandName == InCommandName)
					return std::dynamic_pointer_cast<T>(Command);
			}
		}
		return nullptr;
	}

	Tab() {}
	Tab(std::string InName)
		: Name(InName) {}
};

/*
	The menu root is the top bar of the GUI.
	The tabs are then below and on the left of the menu root.
	The tab features are then to the right of the tabs.

	[       Menu Root     ]
	[ Tab 1 ] [ Feature 1 ]
	[ Tab 2 ] [ Feature 2 ]
	[ Tab 3 ] [ Feature 3 ]
			  [ Feature 4 ]
*/
class MenuRoot
{
private:
	float PencilX, PencilY; // Current drawing position

public:
	float X = 0, Y = 0; // Top left corner of the menu

#ifdef DLL
	bool UserWantsToUnload = false;
#endif

	const float MenuWidth = 600;
	const float RootHeight = 24;
	const float RootPadding = 5;
	int SelectedTabIndex = 0;
	int SelectedCommandIndex = 0;
	const char* AddressDenominator = " / ";

	std::vector<std::shared_ptr<Tab>> Tabs;

	GUIRenderer GUI;

	void DrawRoot();
	void HandleInput(class InputHandler& Input);

	std::string GetAddressText();

	bool DoesSelectedCommandIndexExist(); // checks for your command index in the current tab or list

	bool DoesSelectedTabIndexExist()
	{
		return SelectedTabIndex >= 0 && SelectedTabIndex < Tabs.size();
	}

	std::shared_ptr<Tab> GetTabByName(const std::string& InName)
	{
		for (auto& Tab : Tabs)
		{
			if (Tab->Name == InName)
				return Tab;
		}
		return nullptr;
	}

	// searches all tabs for a command by display name
	template<typename T>
	std::shared_ptr<T> GetCommandByName(const std::string& InName)
	{
		for (auto& Tab : Tabs)
		{
			if (auto& Command = Tab->GetCommandByName<T>(InName))
				return Command;
		}
		return nullptr;
	}

	// search all tabs for a command by any of its command names
	template<typename T>
	std::shared_ptr<T> GetCommandByCommandName(const std::string& InCommandName)
	{
		for (auto& Tab : Tabs)
		{
			if (auto& Command = Tab->GetCommandByName<T>(InCommandName))
				return Command;
		}
		return nullptr;
	}

	std::shared_ptr<Command> GetSelectedCommand(); // returns the currently selected command in the current tab or list
};

extern MenuRoot Menu;