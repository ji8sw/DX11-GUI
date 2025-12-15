#pragma once
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

class TextRenderer
{
private:
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;
	std::unique_ptr<DirectX::CommonStates> m_States;

public:
	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, const wchar_t* SpritefontFile);
	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, unsigned char* FontData, unsigned int FontDataLength);
	void BeginFrame();
	void EndFrame();

	// Drawing Functions
	void Draw(const char* Text, float X, float Y, DirectX::XMVECTORF32 Colour, float Scale = 1.0f);
	float GetTextWidth(const char* Text, float Scale = 1.0f);
};