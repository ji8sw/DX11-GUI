#pragma once
#include "MenuRoot.hpp"

namespace Tabs::Online
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Online";



		return This;
	}
}