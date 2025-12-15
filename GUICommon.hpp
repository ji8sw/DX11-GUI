#pragma once

#include "Shapes.hpp"
#include "Text.hpp"
#include "Texture.hpp"
#include <unordered_map>
#include <string>
#include <vector>

class GUIRenderer
{
private:

#ifdef DLL
	HMODULE m_Module = nullptr;
#endif

	ShapeRenderer m_ShapeRenderer;
	TextRenderer m_TextRenderer;
	std::unordered_map<std::string, TextureRenderer> m_TextureRenderers;

	/*
		Text is supposed to be drawn at a specific point in rendering, so we queue it and draw it at the end of the frame.
	*/
	struct TextDrawCommand
	{
		std::string Text;
		float X;
		float Y;
		DirectX::XMVECTORF32 Colour;
		float Scale = 1.0f;
	};
	std::vector<TextDrawCommand> TextQueue;

	/*
		Textures are supposed to be drawn after everything else
	*/
	struct TextureDrawCommand
	{
		std::string Name;
		float X;
		float Y;
		DirectX::XMVECTORF32 Colour;
		float ScaleX = 1.0f;
		float ScaleY = 1.0f;
	};
	std::vector<TextureDrawCommand> TextureQueue;

	// DX Objects
	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_Context = nullptr;

public:
	ID3D11Device* GetDevice() { return m_Device; }
	ID3D11DeviceContext* GetContext() { return m_Context; }

	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, int Width, int Height);
	void BeginFrame(ID3D11DeviceContext* Context);
	void EndFrame();
	void OnResize(int Width, int Height);

	HMODULE GetModule();
	void ProvideModule(HMODULE Module)
	{
#ifdef DLL
		m_Module = Module;
#endif
	}

	// Shape Drawing Functions
	inline void DrawLine(float StartX, float StartY, float EndX, float EndY, DirectX::XMVECTORF32 Colour)
	{ m_ShapeRenderer.DrawLine(StartX, StartY, EndX, EndY, Colour); }

	inline void DrawFilledRect(float X, float Y, float Width, float Height, DirectX::XMVECTORF32 Colour)
	{ m_ShapeRenderer.DrawFilledRect(X, Y, Width, Height, Colour); }

	inline void DrawRect(float X, float Y, float Width, float Height, DirectX::XMVECTORF32 Colour)
	{ m_ShapeRenderer.DrawRect(X, Y, Width, Height, Colour); }

	inline void DrawFilledTriangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, DirectX::XMVECTORF32 Colour)
	{ m_ShapeRenderer.DrawFilledTriangle(X1, Y1, X2, Y2, X3, Y3, Colour); }

	inline void DrawTriangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, DirectX::XMVECTORF32 Colour)
	{ m_ShapeRenderer.DrawTriangle(X1, Y1, X2, Y2, X3, Y3, Colour); }

	// Text Drawing Functions
	inline void AddText(const char* Text, float X, float Y, DirectX::XMVECTORF32 Colour, float Scale = 1.0f)
	{ 
		TextQueue.push_back({ Text, X, Y, Colour, Scale });
	}

	float GetTextWidth(const char* Text, float Scale = 1.0f)
	{
		return m_TextRenderer.GetTextWidth(Text, Scale);
	}

	// Texture Drawing Functions
	TextureRenderer& LoadTexture(const std::string& Name, ID3D11Device* Device, ID3D11DeviceContext* Context, const wchar_t* TexturePath)
	{
		auto& TextureRenderer = m_TextureRenderers[Name];
		TextureRenderer.Initialize(Device, Context, TexturePath);
		return TextureRenderer;
	}

	TextureRenderer& LoadTextureFromMemory(const std::string& Name, ID3D11Device* Device, ID3D11DeviceContext* Context, const uint8_t* Data, size_t Size)
	{
		auto& TextureRenderer = m_TextureRenderers[Name];
		TextureRenderer.InitializeFromMemory(Device, Context, Data, Size);
		return TextureRenderer;
	}

	TextureRenderer* GetTexture(const std::string& Name)
	{
		auto It = m_TextureRenderers.find(Name);
		if (It != m_TextureRenderers.end())
			return &It->second;
		return nullptr;
	}

	void AddTexture(const std::string& Name, float X, float Y, DirectX::XMVECTORF32 Colour, float ScaleX, float ScaleY)
	{
		TextureQueue.push_back({ Name, X, Y, Colour, ScaleX, ScaleY });
	}

	inline void AddTexture(const std::string& Name, float X, float Y, DirectX::XMVECTORF32 Colour = { 1, 1, 1, 1 }, float Scale = 1.0f)
	{
		AddTexture(Name, X, Y, Colour, Scale, Scale);
	}
};