#ifndef DLL
#include "Windows.h"
#include <windowsx.h>
#include "dwmapi.h"
#include "d3d11.h"
#include "string"
#include "iomanip"
#include <iostream>
#include <fstream>
#include "MenuRoot.hpp"
#include "Logger.hpp"
#include "MenuInput.hpp"
#include "Timer.hpp" // for framerate monitoring
#include "Files.hpp"

// Tabs
#include "TabSelf.hpp"
#include "TabVehicle.hpp"
#include "TabOnline.hpp"
#include "TabPlayers.hpp"
#include "TabWorld.hpp"
#include "TabGame.hpp"
#include "TabStand.hpp"

#define TRANSPARENT_WINDOW
#ifdef TRANSPARENT_WINDOW
POINTS WindowPosition = { 0, 0 };
#endif

LRESULT WINAPI WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0L;
    case WM_SIZE:
        if (auto Context = Menu.GUI.GetContext())
        {
			D3D11_VIEWPORT VP{};
			VP.Width = LOWORD(LParam);
			VP.Height = HIWORD(LParam);
			VP.MinDepth = 0.0f;
			VP.MaxDepth = 1.0f;
			VP.TopLeftX = 0;
			VP.TopLeftY = 0;
			Context->RSSetViewports(1, &VP);
        }
		Menu.GUI.OnResize(LOWORD(LParam), HIWORD(LParam));
		return 0L;
#ifdef TRANSPARENT_WINDOW
    case WM_MOUSEMOVE:
        if (WParam == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(LParam);
            auto rect = RECT{ };

            GetWindowRect(Window, &rect);

            rect.left += points.x - WindowPosition.x;
            rect.top += points.y - WindowPosition.y;

            auto WindowSize = Menu.GUI.GetWindowSize();
            rect.left -= WindowSize.x / 2;
			rect.top -= WindowSize.y / 2;

            SetWindowPos(
                Window,
                HWND_TOPMOST,
                rect.left, rect.top,
                0, 0,
                SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
            );
        }
#endif
    }

    return DefWindowProc(Window, Message, WParam, LParam);
}

INT APIENTRY WinMain(HINSTANCE Instance, HINSTANCE, PSTR, INT ShowCMD)
{
    WNDCLASSEXW WC = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, WideMenuName, NULL};

    RegisterClassExW(&WC);

	int WindowWidth = 650;
	int WindowHeight = 540;

    auto Window = CreateWindowExW(
        0,
        WC.lpszClassName,
        WC.lpszClassName,
#ifdef TRANSPARENT_WINDOW
        WS_POPUP,
#else
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
#endif
        100,
        100,
        WindowWidth, // X
        WindowHeight, // Y
        nullptr,
        nullptr,
        WC.hInstance,
        nullptr
    );

#ifdef TRANSPARENT_WINDOW
    MARGINS M = { -1 };
    DwmExtendFrameIntoClientArea(Window, &M);
#endif

    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.SampleDesc.Count = 1U;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = Window;
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.BufferCount = 2;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL FeatureLevels[]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    ID3D11Device* Device{ nullptr };
    ID3D11DeviceContext* DeviceContext{ nullptr };
    IDXGISwapChain* SwapChain{ nullptr };
    ID3D11RenderTargetView* RenderTargetView{ nullptr };
    D3D_FEATURE_LEVEL FeatureLevel{};

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0U,
        FeatureLevels,
        2U,
        D3D11_SDK_VERSION,
        &SwapChainDesc,
        &SwapChain,
        &Device,
        &FeatureLevel,
        &DeviceContext
    );

    ID3D11Texture2D* BackBuffer{ nullptr };
    SwapChain->GetBuffer(0U, IID_PPV_ARGS(&BackBuffer));

    if (BackBuffer)
    {
        Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderTargetView);
        BackBuffer->Release();
    }
    else
        return 1;

    ShowWindow(Window, ShowCMD);
    UpdateWindow(Window);

#ifdef TRANSPARENT_WINDOW
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
    DepthStencilDesc.DepthEnable = FALSE;
    DepthStencilDesc.StencilEnable = FALSE;
    ID3D11DepthStencilState* DSState = nullptr;
    Device->CreateDepthStencilState(&DepthStencilDesc, &DSState);
    DeviceContext->OMSetDepthStencilState(DSState, 0);

    D3D11_BLEND_DESC BlendDesc = {};
    BlendDesc.RenderTarget[0].BlendEnable = FALSE;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ID3D11BlendState* BlendState = nullptr;
    Device->CreateBlendState(&BlendDesc, &BlendState);
    DeviceContext->OMSetBlendState(BlendState, nullptr, 0xffffffff);
#endif

    D3D11_RASTERIZER_DESC RastDesc = {};
    RastDesc.ScissorEnable = FALSE;
    RastDesc.FillMode = D3D11_FILL_SOLID;
    RastDesc.CullMode = D3D11_CULL_NONE;
    ID3D11RasterizerState* RasterState = nullptr;
    Device->CreateRasterizerState(&RastDesc, &RasterState);
    DeviceContext->RSSetState(RasterState);

    D3D11_VIEWPORT VP{};
    VP.Width = WindowWidth;
    VP.Height = WindowHeight;
    VP.MinDepth = 0.0f;
    VP.MaxDepth = 1.0f;
    VP.TopLeftX = 0;
    VP.TopLeftY = 0;
    DeviceContext->RSSetViewports(1, &VP);

#ifndef NDEBUG
    CreateConsole();
#endif

	InputHandler MenuInput;
    MenuInput.RegisterStandardHotkeys();

    Menu.GUI.Initialize(Device, DeviceContext, WindowWidth, WindowHeight);

	auto ListTexData = Files::LoadResource(MAKEINTRESOURCE(IDB_PNG1), L"PNG");
	Menu.GUI.LoadTextureFromMemory("List", Device, DeviceContext, (const uint8_t*)ListTexData.Data, ListTexData.Size);

    auto ToggleOffTexData = Files::LoadResource(MAKEINTRESOURCE(IDB_PNG2), L"PNG");
    Menu.GUI.LoadTextureFromMemory("Toggle Off", Device, DeviceContext, (const uint8_t*)ToggleOffTexData.Data, ToggleOffTexData.Size);

    auto ToggleOnTexData = Files::LoadResource(MAKEINTRESOURCE(IDB_PNG3), L"PNG");
    Menu.GUI.LoadTextureFromMemory("Toggle On", Device, DeviceContext, (const uint8_t*)ToggleOnTexData.Data, ToggleOnTexData.Size);

	Menu.Tabs.push_back(Tabs::Self::Create());
	Menu.Tabs.push_back(Tabs::Vehicle::Create());
	Menu.Tabs.push_back(Tabs::Online::Create());
	Menu.Tabs.push_back(Tabs::Players::Create());
	Menu.Tabs.push_back(Tabs::World::Create());
	Menu.Tabs.push_back(Tabs::Game::Create());
	Menu.Tabs.push_back(Tabs::Stand::Create());

    bool Running = true;
	bool MenuOpen = true;
	Timer FrameTimer;

    while (Running)
    {
		float FrameTime = FrameTimer.GetElapsedMilliseconds();
        FrameTimer.Start();

        MSG Message;
        while (PeekMessage(&Message, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);

            if (Message.message == WM_QUIT)
                Running = false;
        }

        if (!Running)
            break;

#ifdef TRANSPARENT_WINDOW
        constexpr float Colour[4]{ 0.0f, 0.0f, 0.0f, 0.f };
#else
        constexpr float Colour[4]{ 0.1f, 0.1f, 0.1f, 1.f };
#endif
        DeviceContext->OMSetRenderTargets(1U, &RenderTargetView, nullptr);
        DeviceContext->ClearRenderTargetView(RenderTargetView, Colour);

        if (MenuInput.Hotkeys["MenuToggle"].WasReleased())
			MenuOpen = !MenuOpen;

        Menu.GUI.BeginFrame(DeviceContext);

        if (MenuOpen)
        {
            if (GetForegroundWindow() == Window)
                Menu.HandleInput(MenuInput);
            Menu.DrawRoot();
        }

		{ // FPS Counter
			auto Tab = Menu.GetTabByName("Stand");
            if (Tab)
            {
				auto FPSCommand = Tab->GetCommandByName<Toggle>("Show FPS");
                if (FPSCommand && FPSCommand->GetType() == CT_Toggle)
                {
					auto FPSToggle = std::dynamic_pointer_cast<Toggle>(FPSCommand);
                    if (FPSToggle && FPSToggle->GetValue())
                    {
                        std::string FPSText = std::format("FPS: {:.0f}", 1000.0f / FrameTime);
						float TextWidth = Menu.GUI.GetTextWidth(FPSText.c_str(), 0.5f);
                        Menu.GUI.AddText(FPSText.c_str(), WindowWidth - (TextWidth * 1.3), WindowHeight - 60, {1, 1, 1, 1}, 0.5f);
                    }
                }
            }
        }

        Menu.GUI.EndFrame();

        SwapChain->Present(1U, 0U);
    }

    if (SwapChain)
        SwapChain->Release();
    if (DeviceContext)
        DeviceContext->Release();
    if (Device)
        Device->Release();
    if (RenderTargetView)
        RenderTargetView->Release();

    DestroyWindow(Window);
    UnregisterClassW(WC.lpszClassName, WC.hInstance);

    return 0;
}
#endif
