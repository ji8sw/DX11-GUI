#pragma once
#include "Action.hpp"
#include "List.hpp"
#include "Slider.hpp"
#include "Toggle.hpp"
#include "Divider.hpp"
#include "MenuRoot.hpp"

#define PUSH_EMPTY_LIST(VarName, Name) \
{ \
	std::unique_ptr<List> VarName = std::make_unique<List>(Name); \
	This->Add<List>(std::move(VarName)); \
}