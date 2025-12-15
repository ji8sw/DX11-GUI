#pragma once
#include "CommandTypeIncludes.hpp"

namespace Tabs::Players
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Players";

		PUSH_EMPTY_LIST(SettingsList, "Settings");
		PUSH_EMPTY_LIST(AllPlayersList, "All Players");

		std::unique_ptr<Divider> Separator = std::make_unique<Divider>("Players (4)");
		This->Add<Divider>(std::move(Separator));

		std::unique_ptr<List> PlayerMenu = std::make_unique<List>("GTAdev1");
		{
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Information")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Teleport")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Friendly")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Chat")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Weapons")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Trolling")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Toggle>("Waypoint")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Copy Their Waypoint")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Spectate")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Toggle>("Show Camera")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Open Profile")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Send Friend Request")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List> ("In Player History")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Copy Outfit")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Copy Vehicle")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Issue Commands On Their Behalf")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Toggle>("Join CEO/MC")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Command>("Invite To Pong")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Increment Commend/Report Stats")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Kick")));
			auto CrashList = std::make_unique<List>("Crash");
			{
				CrashList->Commands.push_back(std::move(std::make_unique<Command>("Elegant")));
				CrashList->Commands.push_back(std::move(std::make_unique<Command>("Vehicular Manslaughter")));
			}
			PlayerMenu->Commands.push_back(std::move(CrashList));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<Toggle>("Timeout")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Network Events")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Incoming Syncs")));
			PlayerMenu->Commands.push_back(std::move(std::make_unique<List>("Outgoing Syncs")));
		}

		List RawCopy = *PlayerMenu;
		std::unique_ptr<List> PlayerEntry1 = std::make_unique<List>(RawCopy);
		PlayerEntry1->Name = "GTAdev1";
		This->Add<List>(std::move(PlayerEntry1));

		std::unique_ptr<List> PlayerEntry2 = std::make_unique<List>(RawCopy);
		PlayerEntry2->Name = "GTAdev2";
		This->Add<List>(std::move(PlayerEntry2));

		std::unique_ptr<List> PlayerEntry3 = std::make_unique<List>(RawCopy);
		PlayerEntry3->Name = "GTAdev3";
		This->Add<List>(std::move(PlayerEntry3));

		std::unique_ptr<List> PlayerEntry4 = std::make_unique<List>(RawCopy);
		PlayerEntry4->Name = "GTAdev4";
		This->Add<List>(std::move(PlayerEntry4));

		return This;
	}
}