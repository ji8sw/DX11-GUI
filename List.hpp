#pragma once
#include "MenuRoot.hpp"

/*
	Lists are commands that when clicked, will open a submenu that contains more commands.
	They work like Tabs but don't have their own tab button, and are instead opened from within a tab.
	For appearance, they look like commands but have an arrow on the right side to indicate they open a submenu.
*/
class List : public Command
{
public:
	std::vector<std::shared_ptr<Command>> Commands;

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

	void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered) override;

	virtual void OnClick() override;

	List(std::string InName)
		: Command(InName) {}

	CommandType GetType() override { return CT_List; }
};