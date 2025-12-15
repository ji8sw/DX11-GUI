#pragma once
#include "MenuRoot.hpp"

namespace Tabs::Vehicle
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Vehicle";



		return This;
	}
}