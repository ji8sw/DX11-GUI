#pragma once
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <wrl/client.h>
#include <CommonStates.h>

class TextureRenderer
{
private:
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	std::unique_ptr<DirectX::CommonStates> m_States;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

public:
	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, const wchar_t* TexturePath);
	void InitializeFromMemory(ID3D11Device* Device, ID3D11DeviceContext* Context, const uint8_t* Data, size_t Size);
	void BeginFrame();
	void EndFrame();

	// Drawing Functions
	void Draw(float X, float Y, DirectX::XMVECTORF32 Colour, float ScaleX, float ScaleY);
	inline void Draw(float X, float Y, DirectX::XMVECTORF32 Colour = { 1, 1, 1, 1 }, float Scale = 1.0f);

	// Get Size
	DirectX::XMFLOAT2 GetTextureSize(ID3D11Device* Device);
};