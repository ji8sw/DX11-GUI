#pragma once
#include "MenuRoot.hpp"
#include "Toggle.hpp"

namespace Tabs::Stand
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Stand";

		std::unique_ptr<Toggle> ShowFPSToggle = std::make_unique<Toggle>("Show FPS");
		This->Add<Toggle>(std::move(ShowFPSToggle));

#ifdef DLL
		std::unique_ptr<Command> UnloadCommand = std::make_unique<Command>("Unload");
		UnloadCommand->OnClickCallback = []()
		{
			Menu.UserWantsToUnload = true;
		};
		This->Add<Command>(std::move(UnloadCommand));
#endif

		return This;
	}
}