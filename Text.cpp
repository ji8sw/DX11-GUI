#include "Text.hpp"
#include <filesystem> // ensuring that the font file exists
#include <stdexcept>

void TextRenderer::Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, const wchar_t* SpritefontFile)
{
	if (!std::filesystem::exists(SpritefontFile))
		throw std::runtime_error("SpriteFont file not found");

	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(Context);
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(Device, SpritefontFile);
	m_States = std::make_unique<DirectX::CommonStates>(Device);
}

void TextRenderer::Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, unsigned char* FontData, unsigned int FontDataLength)
{
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(Context);
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(Device, FontData, FontDataLength);
	m_States = std::make_unique<DirectX::CommonStates>(Device);
}

void TextRenderer::BeginFrame()
{
	m_SpriteBatch->Begin(
		DirectX::DX11::SpriteSortMode_Deferred,
		m_States->AlphaBlend(),
		m_States->LinearClamp(),
		m_States->DepthNone(),
		m_States->CullNone()
	);
}

void TextRenderer::EndFrame()
{
	m_SpriteBatch->End();
}

void TextRenderer::Draw(const char* Text, float X, float Y, DirectX::XMVECTORF32 Colour, float Scale)
{
	X = std::floorf(X) + 0.5f;
	Y = std::floorf(Y) + 0.5f;

	m_SpriteFont->DrawString(
		m_SpriteBatch.get(),
		Text,
		DirectX::XMFLOAT2(X, Y),
		Colour,
		0.0f,
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(Scale, Scale)
	);
}

float TextRenderer::GetTextWidth(const char* Text, float Scale)
{
	auto Size = m_SpriteFont->MeasureString(Text);
	return DirectX::XMVectorGetX(DirectX::XMVectorScale(Size, Scale));
}