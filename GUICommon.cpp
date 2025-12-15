#include "GUICommon.hpp"
#include "Files.hpp"
#include "Logger.hpp"

void GUIRenderer::Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, int Width, int Height)
{
	m_Device = Device;
	m_Context = Context;
	m_Width = Width;
	m_Height = Height;

	m_ShapeRenderer.Initialize(m_Device, m_Context, m_Width, m_Height);

	// Load font
	auto ListTextureData = Files::LoadResource(MAKEINTRESOURCE(IDB_FONT1), MAKEINTRESOURCE(RT_RCDATA));
	m_TextRenderer.Initialize(m_Device, m_Context, (unsigned char*)ListTextureData.Data, ListTextureData.Size);
}

void GUIRenderer::BeginFrame(ID3D11DeviceContext* Context)
{
	m_ShapeRenderer.BeginFrame(Context);
	m_TextRenderer.BeginFrame();
}

void GUIRenderer::EndFrame()
{
	for (const auto& Command : m_TextQueue)
	{
		m_TextRenderer.Draw(Command.Text.c_str(), Command.X, Command.Y, Command.Colour, Command.Scale);
	}
	m_TextQueue.clear();

	m_ShapeRenderer.EndFrame();
	m_TextRenderer.EndFrame();

	for (const auto& Command : m_TextureQueue)
	{
		auto It = m_TextureRenderers.find(Command.Name);
		if (It != m_TextureRenderers.end())
		{
			It->second.BeginFrame();
			It->second.Draw(Command.X, Command.Y, Command.Colour, Command.ScaleX, Command.ScaleY);
			It->second.EndFrame();
		}
	}
	m_TextureQueue.clear();
}

void GUIRenderer::OnResize(int Width, int Height)
{
	m_ShapeRenderer.OnResize(Width, Height);
}

HMODULE GUIRenderer::GetModule()
{
#ifdef DLL
	return m_Module;
#else
	return GetModuleHandle(NULL);
#endif
}
