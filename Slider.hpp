#pragma once
#include "MenuRoot.hpp"

/*
	When hovered, the menu redirects left and right arrow keys to adjust the slider value.
*/
class Slider : public Command
{
private:
	int MinValue = 1;
	int MaxValue = 100;
	int CurrentValue = 0;

public:
	void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered) override;

	virtual void OnLeftAdjust();
	virtual void OnRightAdjust();

	Slider(std::string InName, int InMinValue = 1, int InMaxValue = 100, int InCurrentValue = 50)
		: Command(InName), MinValue(InMinValue), MaxValue(InMaxValue), CurrentValue(InCurrentValue) {}

	CommandType GetType() override { return CT_Slider; }
};

class SliderFloat : public Slider
{
private:
	float fMinValue = 0.0f;
	float fMaxValue = 100.0f;
	float fCurrentValue = 0.0f;
	float Step = 1.0f;

public:
	void Draw(class GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered) override;

	void OnLeftAdjust() override;
	void OnRightAdjust() override;

	SliderFloat(std::string InName, float InMinValue = 0.0f, float InMaxValue = 10.0f, float InCurrentValue = 5.0f, float Step = 0.1f)
		: Slider(InName), fMinValue(InMinValue), fMaxValue(InMaxValue), fCurrentValue(InCurrentValue) {
	}

	CommandType GetType() override { return CT_Slider; }
};