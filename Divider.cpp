#include "Divider.hpp"

void Divider::Draw(GUIRenderer& GUI, float& PencilX, float& PencilY, float RemainingWidth, bool Hovered)
{
	GUI.DrawFilledRect(PencilX, PencilY, RemainingWidth, Height, Hovered ? Menu.ActiveColour : Menu.NormalColour);

	float TextWidth = GUI.GetTextWidth(Name.c_str(), COMMAND_TEXT_SIZE);
	const float DesiredWidth = 32.0f;
	float ScaleFactor = 1.0f;
	if (TextWidth > DesiredWidth)
		ScaleFactor = DesiredWidth / TextWidth;

	float TextX = PencilX + (RemainingWidth * 0.5f);
	TextX -= (TextWidth + 12) / 2;

	GUI.AddText(Name.c_str(), TextX, PencilY + 2, { 1, 1, 1, 1 }, COMMAND_TEXT_SIZE);
	PencilY += Height;
}