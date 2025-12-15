#pragma once
#include <windows.h>
#include <iostream>
#include "MenuRoot.hpp" // for MenuName

namespace Logger
{
	static void CreateConsole()
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	}

	static void LOG(const std::string& Msg)
	{
		std::cout << std::format("[{}]: {}", MenuName, Msg) << std::endl;
	}
}

using namespace Logger;