# DX11 GUI

Easily create GUI using DirectX11 and DirectXTK

DX11 GUI is a simple library built on top of DirectX11 and DirectXTK to facilitate the creation of graphical user interfaces in DirectX11 applications. It provides a set of tools and components to help developers create menus, buttons, sliders, and other UI elements with ease.

DX11 GUI can be built as a standalone `.exe` or as an injectable `.dll` for use in games and other applications.

DX11 GUI already provides a UI and when built, can immediately be injected into a DirectX11 application to display the GUI.

You can completely scrap the existing UI and use the library to create your own custom GUI.
If you want to create a standalone `.exe`, you are welcome to delete `Kiero\` and `DLLMain.cpp` as they are only needed for DLL injection.

## Setup

1. Clone or download the repository.
2. Download and install [DirectXTK from its GitHub repository](https://github.com/microsoft/DirectXTK)
3. Compile DXTK for Desktop x64 using Visual Studio.
3. Open the DX11 GUI solution in Visual Studio.
4. Change the build configuration to `Release` or `DLL Release` as needed.
5. Build the solution.

## Notes

- Already included is a recreation of the list UI from [Stand](https://stand.sh) for GTA 5.
- The `Resources\` folder contains necessary textures and fonts, they are integrated into the project as resources, therefore you don't need to ship them.
- If you wish you can swap out textures and fonts with your own either by changing resources or by loading them as files at runtime.