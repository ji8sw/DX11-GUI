#include "Texture.hpp"
#include <stdexcept>

void TextureRenderer::Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, const wchar_t* TexturePath)
{
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(Context);
	m_States = std::make_unique<DirectX::CommonStates>(Device);

	HRESULT Result = DirectX::CreateWICTextureFromFile(
		Device,
		TexturePath,
		nullptr,
		m_Texture.GetAddressOf()
	);

	if (FAILED(Result)) throw std::runtime_error("Failed to load texture from file.");
}

void TextureRenderer::InitializeFromMemory(ID3D11Device* Device, ID3D11DeviceContext* Context, const uint8_t* Data, size_t Size)
{
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(Context);
	m_States = std::make_unique<DirectX::CommonStates>(Device);
	HRESULT Result = DirectX::CreateWICTextureFromMemory(
		Device,
		Data,
		Size,
		nullptr,
		m_Texture.GetAddressOf()
	);
	if (FAILED(Result)) throw std::runtime_error("Failed to load texture from memory.");
}

void TextureRenderer::BeginFrame()
{
	using namespace DirectX;
	m_SpriteBatch->Begin(
		DX11::SpriteSortMode_Deferred,
		m_States->AlphaBlend(),
		m_States->LinearClamp(),
		m_States->DepthNone(),
		m_States->CullNone()
	);
}

void TextureRenderer::EndFrame()
{
	m_SpriteBatch->End();
}

void TextureRenderer::Draw(float X, float Y, DirectX::XMVECTORF32 Colour, float ScaleX, float ScaleY)
{
	m_SpriteBatch->Draw(
		m_Texture.Get(),
		DirectX::XMFLOAT2(X, Y),
		nullptr,
		Colour,
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0),
		DirectX::XMFLOAT2(ScaleX, ScaleY)
	);
}

void TextureRenderer::Draw(float X, float Y, DirectX::XMVECTORF32 Colour, float Scale)
{
	Draw(X, Y, Colour, Scale, Scale);
}

DirectX::XMFLOAT2 TextureRenderer::GetTextureSize(ID3D11Device* Device)
{
	if (!Device) return DirectX::XMFLOAT2(0.0f, 0.0f);

	D3D11_TEXTURE2D_DESC Desc;
	Microsoft::WRL::ComPtr<ID3D11Resource> Resource;
	m_Texture->GetResource(Resource.GetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;
	Resource.As(&Texture2D);
	Texture2D->GetDesc(&Desc);
	return DirectX::XMFLOAT2(static_cast<float>(Desc.Width), static_cast<float>(Desc.Height));
}