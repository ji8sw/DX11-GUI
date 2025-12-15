#pragma once
#include "MenuRoot.hpp"

/*
	Simple toggle command, can be on or off.
*/
class Toggle : public Command
{
private:
	bool Value = false;

public:
	void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered) override;

	void OnClick() override;

	Toggle(std::string InName, bool Default = false)
		: Command(InName), Value(Default) {
	}

	CommandType GetType() override { return CT_Toggle; }
	bool GetValue() const { return Value; }
};