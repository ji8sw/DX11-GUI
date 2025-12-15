#pragma once
#include "MenuRoot.hpp"

namespace Tabs::Game
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Game";



		return This;
	}
}