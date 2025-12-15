#pragma once
#include "MenuRoot.hpp"

namespace Tabs::Players
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Players";



		return This;
	}
}