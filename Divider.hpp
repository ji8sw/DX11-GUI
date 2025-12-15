#pragma once
#include "MenuRoot.hpp"

/*
	A divider is a non-interactive command that separates sections of commands.
	It can contain text, which is centered in the divider.
	When encountered during navigation, it is skipped over.
*/
class Divider : public Command
{
public:
	void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered) override;

	void OnClick() override {};

	CommandType GetType() override { return CT_Divider; }

	Divider(std::string InName = "")
		: Command(InName) {
	}
};