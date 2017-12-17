#pragma once

#include <queue>

#include "UnitManagement.hpp"
#include "Hashes.hpp"

namespace ZerGreenAI
{
	class ProductionManager : public UnitManager
	{
		constexpr static int TIED_DOWN_DURATION = 0;
		constexpr static int CANCEL_TIMER_DURATION = 24;

		Unitset idleBuildings;
		std::map<BWAPI::Unit, int> latencyFrames;
		std::map<BWAPI::UnitType, int> tiedDownBuildingTypes;
		void onFrame() override;
		bool isIdleBuilding(BWAPI::Unit u);
	public:
		inline virtual std::string name() { return "Production Manager"; }
		std::vector<BWAPI::UnitType> unassignedBuildingTypes();
		bool produceType(BWAPI::UnitType whatType);
		void tieDown(BWAPI::UnitType whatType);
	};
}