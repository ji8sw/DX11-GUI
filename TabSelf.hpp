#pragma once
#include "MenuRoot.hpp"
#include "List.hpp"
#include "Slider.hpp"
#include "Toggle.hpp"

namespace Tabs::Self
{
	std::unique_ptr<Tab> Create()
	{
		auto This = std::make_unique<Tab>("Self");

		std::unique_ptr<List> MovementList = std::make_unique<List>("Movement");
		{
			std::unique_ptr<List> LevitationList = std::make_unique<List>("Levitation");
			MovementList->Commands.push_back(std::move(LevitationList));

			std::unique_ptr<SliderFloat> MovementSpeed = std::make_unique<SliderFloat>("Movement Speed", 0.01, 10000.0f, 1.0f);
			MovementList->Commands.push_back(std::move(MovementSpeed));

			std::unique_ptr<Command> SprintMult = std::make_unique<Command>("Sprint Multiplier");
			MovementList->Commands.push_back(std::move(SprintMult));
		}
		This->Add<List>(std::move(MovementList));

		std::unique_ptr<List> AppearanceList = std::make_unique<List>("Appearance");
		This->Add<List>(std::move(AppearanceList));

		std::unique_ptr<List> WeaponsList = std::make_unique<List>("Weapons");
		This->Add<List>(std::move(WeaponsList));

		std::unique_ptr<Toggle> ImmortalityToggle = std::make_unique<Toggle>("Immortality");
		This->Add<Toggle>(std::move(ImmortalityToggle));

		std::unique_ptr<Toggle> AutoHealToggle = std::make_unique<Toggle>("Auto Heal");
		This->Add<Toggle>(std::move(AutoHealToggle));

		std::unique_ptr<Toggle> GracefullnessToggle = std::make_unique<Toggle>("Gracefulness");
		This->Add<Toggle>(std::move(GracefullnessToggle));

		std::unique_ptr<Toggle> GluedToSeatsToggle = std::make_unique<Toggle>("Glued To Seats");
		This->Add<Toggle>(std::move(GluedToSeatsToggle));

		std::unique_ptr<Slider> SetWantedLevelSlider = std::make_unique<Slider>("Set Wanted Level", 0, 5, 0);
		This->Add<Slider>(std::move(SetWantedLevelSlider));

		std::unique_ptr<Toggle> LockWantedLevelToggle = std::make_unique<Toggle>("Lock Wanted Level");
		This->Add<Toggle>(std::move(LockWantedLevelToggle));

		std::unique_ptr<Slider> FakeWantedLevelSlider = std::make_unique<Slider>("Fake Wanted Level", 0, 5, 0);
		This->Add<Slider>(std::move(FakeWantedLevelSlider));

		std::unique_ptr<Toggle> InfiniteParachutesToggle = std::make_unique<Toggle>("Infinite Parachutes");
		This->Add<Toggle>(std::move(InfiniteParachutesToggle));

		std::unique_ptr<List> SetStoryModeCashList = std::make_unique<List>("Set Story Mode Cash");
		This->Add<List>(std::move(SetStoryModeCashList));

		std::unique_ptr<Toggle> ClumsinessToggle = std::make_unique<Toggle>("Clumsiness");
		This->Add<Toggle>(std::move(ClumsinessToggle));

		std::unique_ptr<Command> SuicideCommand = std::make_unique<Command>("Suicide");
		This->Add<Command>(std::move(SuicideCommand));

		return This;
	}
}