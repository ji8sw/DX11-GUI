#pragma once
#include "MenuRoot.hpp"

namespace Tabs::World
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "World";



		return This;
	}
}