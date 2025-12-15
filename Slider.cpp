#include "Slider.hpp"
#include "Logger.hpp"

void Slider::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, RemainingWidth, Height, Hovered ? ActiveColour : NormalColour);

	auto Texture = GUI.GetTexture("List");
	auto Device = GUI.GetDevice();
	auto TextureSize = Texture->GetTextureSize(Device);
	float CenterX = TextureSize.x / 2.0f;
	float CenterY = TextureSize.y / 2.0f;
	CenterX *= 0.10f; // scale down
	CenterY *= 0.10f; // scale down
	std::string ValueText = "";
	if (Hovered)
		ValueText = std::format("< {} >", CurrentValue);
	else
		ValueText = std::format("{}", CurrentValue);
	float ValueTextWidth = GUI.GetTextWidth(ValueText.c_str(), COMMAND_TEXT_SIZE);
	const float DesiredWidth = 32.0f;
	float ScaleFactor = 1.0f;
	if (ValueTextWidth > DesiredWidth)
		ScaleFactor = DesiredWidth / ValueTextWidth;

	float ValueTextX = PencilX + (RemainingWidth * 1.f);
	ValueTextX -= ValueTextWidth + 12;

	GUI.AddText(ValueText.c_str(), ValueTextX, PencilY + 2, { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	PencilY += Height;
}

void Slider::OnLeftAdjust()
{
	CurrentValue -= 1;
	if (CurrentValue < MinValue)
		CurrentValue = MaxValue;
}

void Slider::OnRightAdjust()
{
	CurrentValue += 1;
	if (CurrentValue > MaxValue)
		CurrentValue = MinValue;
}

void SliderFloat::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, RemainingWidth, Height, Hovered ? ActiveColour : NormalColour);

	auto Texture = GUI.GetTexture("List");
	auto Device = GUI.GetDevice();
	auto TextureSize = Texture->GetTextureSize(Device);
	float CenterX = TextureSize.x / 2.0f;
	float CenterY = TextureSize.y / 2.0f;
	CenterX *= 0.10f; // scale down
	CenterY *= 0.10f; // scale down
	std::string ValueText = "";
	if (Hovered)
		ValueText = std::format("< {:.2f} >", fCurrentValue);
	else
		ValueText = std::format("{:.2f}", fCurrentValue);
	float ValueTextWidth = GUI.GetTextWidth(ValueText.c_str(), COMMAND_TEXT_SIZE);
	const float DesiredWidth = 32.0f;
	float ScaleFactor = 1.0f;
	if (ValueTextWidth > DesiredWidth)
		ScaleFactor = DesiredWidth / ValueTextWidth;

	float ValueTextX = PencilX + (RemainingWidth * 1.f);
	ValueTextX -= ValueTextWidth + 12;

	GUI.AddText(ValueText.c_str(), ValueTextX, PencilY + 2, { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	PencilY += Height;
}

void SliderFloat::OnLeftAdjust()
{
	fCurrentValue -= Step;
	if (fCurrentValue < fMinValue)
		fCurrentValue = fMaxValue;
}

void SliderFloat::OnRightAdjust()
{
	fCurrentValue += Step;
	if (fCurrentValue > fMaxValue)
		fCurrentValue = fMinValue;
}