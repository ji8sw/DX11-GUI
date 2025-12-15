#include "Toggle.hpp"

void Toggle::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, RemainingWidth, Height, Hovered ? ActiveColour : NormalColour);
	std::string TextureName = Value ? "Toggle On" : "Toggle Off";
	GUI.AddTexture(TextureName.c_str(), PencilX + (RemainingWidth * 0.93f), PencilY, {1, 1, 1, 1}, 0.13f);
	GUI.AddText(Name.c_str(), PencilX, PencilY + (Height / 8), { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	PencilY += Height;
}

void Toggle::OnClick()
{
	Value = !Value;
}