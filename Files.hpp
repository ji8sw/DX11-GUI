#pragma once
#include <filesystem>
#include "resource.h"
typedef wchar_t* LPWSTR;

namespace Files
{
	struct LoadedData
	{
		void* Data;
		size_t Size;
	};

	LoadedData LoadResource(const wchar_t* ResourceName, const wchar_t* ResourceType);
	LoadedData LoadResource(const wchar_t* ResourceName, LPWSTR ResourceType);
	LoadedData LoadResource(LPWSTR ResourceName, LPWSTR ResourceType);
}