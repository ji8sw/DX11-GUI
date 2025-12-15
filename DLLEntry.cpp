#ifdef DLL
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
#include "kiero.h"
#include "Files.hpp"

// Tabs
#include "TabSelf.hpp"
#include "TabVehicle.hpp"
#include "TabOnline.hpp"
#include "TabPlayers.hpp"
#include "TabWorld.hpp"
#include "TabGame.hpp"
#include "TabStand.hpp"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present oPresent;
ID3D11Device* Device = nullptr;
ID3D11DeviceContext* DeviceContext = nullptr;
bool MenuOpen = true;
InputHandler MenuInput;
Timer FrameTimer;

HWND GameWindow = nullptr;
WNDPROC OriginalWndProc = nullptr;

unsigned int WindowWidth = 1920;
unsigned int WindowHeight = 1080;

DWORD WINAPI Uninject(LPVOID Reserved) // WIP
{
    LOG("A");
    Sleep(100); // give some time for the present to finish
    LOG("B");
    SetWindowLongPtr(GameWindow, GWLP_WNDPROC, (LONG_PTR)OriginalWndProc);
    LOG("C");
    LOG("D");
    kiero::shutdown();
    LOG("E");

    if (DeviceContext)
    {
        DeviceContext->ClearState();
        DeviceContext->Flush();
    }
    if (DeviceContext) { DeviceContext->Release(); DeviceContext = nullptr; }
    if (Device) { Device->Release(); Device = nullptr; }
    LOG("F");

    FreeConsole();
    LOG("G");

    HMODULE Mod;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)Uninject, &Mod);
    FreeLibraryAndExitThread(Mod, 0);
    return 0;
}

LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        for (auto& [Name, Hotkey] : MenuInput.Hotkeys)
        {
            if (Hotkey.BlockGameInputWhenMenuOpen && MenuOpen)
                return TRUE;
        }
        break;
    case WM_KEYUP:
        for (auto& [Name, Hotkey] : MenuInput.Hotkeys)
        {
            if (Hotkey.BlockGameInputWhenMenuOpen && MenuOpen)
                return TRUE;
        }
        break;
    }

    return CallWindowProc(OriginalWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	float FrameTime = FrameTimer.GetElapsedMilliseconds();
	FrameTimer.Start();

    static bool Initialized = false;
    if (!Initialized)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<LPVOID*>(&Device))))
        {
            Device->GetImmediateContext(&DeviceContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);

            GameWindow = sd.OutputWindow;
            OriginalWndProc = (WNDPROC)SetWindowLongPtr(GameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

			WindowWidth = sd.BufferDesc.Width;
			WindowHeight = sd.BufferDesc.Height;

            Menu.X = (WindowWidth * 0.1f);
            Menu.Y = (WindowHeight * 0.1f);

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

            MenuInput.RegisterStandardHotkeys();

            Initialized = true;
        }
        else return oPresent(pSwapChain, SyncInterval, Flags);
    }

    if (MenuInput.Hotkeys["MenuToggle"].WasReleased())
        MenuOpen = !MenuOpen;

    Menu.GUI.BeginFrame(DeviceContext);

    if (MenuOpen)
    {
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
                    Menu.GUI.AddText(FPSText.c_str(), WindowWidth - (TextWidth * 1.3), WindowHeight - 60, { 1, 1, 1, 1 }, 0.5f);
                }
            }
        }
    }

    Menu.GUI.EndFrame();

    if (Menu.UserWantsToUnload)
    {
        Menu.UserWantsToUnload = false;
        auto Result = oPresent(pSwapChain, SyncInterval, Flags);
        CreateThread(nullptr, 0, Uninject, nullptr, 0, nullptr);
        return Result;
    }

    return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI HookGraphics(LPVOID Reserved)
{
    CreateConsole();

    bool HookedGraphics = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            HookedGraphics = true;
            kiero::bind(8, (void**)&oPresent, (void*)&hkPresent);
        }
    } while (!HookedGraphics);

    return true;
}

BOOL WINAPI DllMain(HMODULE Mod, DWORD Reason, LPVOID Reserved)
{
    if (Reason == 1)
    {
        DisableThreadLibraryCalls(Mod);
		Menu.GUI.ProvideModule(Mod);

        HANDLE MainThread = CreateThread(nullptr, 0, HookGraphics, Mod, 0, nullptr);
        if (MainThread) CloseHandle(MainThread);
    }
    return TRUE;
}
#endif