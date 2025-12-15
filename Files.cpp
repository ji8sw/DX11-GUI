#include "Files.hpp"
#include <windows.h>
#include "MenuRoot.hpp"

namespace Files
{
	LoadedData LoadResource(const wchar_t* ResourceName, const wchar_t* ResourceType)
	{
		HMODULE Handle = Menu.GUI.GetModule();

		HRSRC ResourceHandle = FindResource(Handle, ResourceName, ResourceType);
		if (!ResourceHandle)
			return { nullptr, 0 };
		HGLOBAL LoadedResource = LoadResource(Handle, ResourceHandle);
		if (!LoadedResource)
			return { nullptr, 0 };
		void* ResourceData = LockResource(LoadedResource);
		if (!ResourceData)
			return { nullptr, 0 };
		size_t ResourceSize = SizeofResource(Handle, ResourceHandle);
		return { ResourceData, ResourceSize };
	}

	LoadedData LoadResource(const wchar_t* ResourceName, LPWSTR ResourceType)
	{
		HMODULE Handle = Menu.GUI.GetModule();

		HRSRC ResourceHandle = FindResource(Handle, ResourceName, ResourceType);
		if (!ResourceHandle)
			return { nullptr, 0 };
		HGLOBAL LoadedResource = LoadResource(Handle, ResourceHandle);
		if (!LoadedResource)
			return { nullptr, 0 };
		void* ResourceData = LockResource(LoadedResource);
		if (!ResourceData)
			return { nullptr, 0 };
		size_t ResourceSize = SizeofResource(Handle, ResourceHandle);
		return { ResourceData, ResourceSize };
	}

	LoadedData LoadResource(LPWSTR ResourceName, LPWSTR ResourceType)
	{
		HMODULE Handle = Menu.GUI.GetModule();

		HRSRC ResourceHandle = FindResource(Handle, ResourceName, ResourceType);
		if (!ResourceHandle)
			return { nullptr, 0 };
		HGLOBAL LoadedResource = LoadResource(Handle, ResourceHandle);
		if (!LoadedResource)
			return { nullptr, 0 };
		void* ResourceData = LockResource(LoadedResource);
		if (!ResourceData)
			return { nullptr, 0 };
		size_t ResourceSize = SizeofResource(Handle, ResourceHandle);
		return { ResourceData, ResourceSize };
	}
}