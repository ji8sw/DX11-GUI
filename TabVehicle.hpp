#pragma once
#include "CommandTypeIncludes.hpp"

namespace Tabs::Vehicle
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>();
		This->Name = "Vehicle";

		PUSH_EMPTY_LIST(SpawnVehicleList, "Spawn Vehicle");
		PUSH_EMPTY_LIST(GarageList, "Garage");
		PUSH_EMPTY_LIST(PersonalVehiclesList, "Personal Vehicles");
		PUSH_EMPTY_LIST(CurrentPersonalVehicleList, "Current Personal Vehicle");
		PUSH_EMPTY_LIST(LastVehicleList, "Last Vehicle");
		PUSH_EMPTY_LIST(LSCustomsList, "Los Santos Customs");
		PUSH_EMPTY_LIST(MovementList, "Movement");
		PUSH_EMPTY_LIST(RocketBoostList, "Rocket Boost");
		PUSH_EMPTY_LIST(CollisionsList, "Collisions");
		PUSH_EMPTY_LIST(OpenCloseDoorsList, "Open/Close Doors");
		PUSH_EMPTY_LIST(LockDoorsList, "Lock Doors");

		return This;
	}
}