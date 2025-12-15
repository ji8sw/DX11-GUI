#pragma once
#include <windows.h>
#include <iostream>

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
		std::cout << "[DX11]: " << Msg << std::endl;
	}
}

using namespace Logger;