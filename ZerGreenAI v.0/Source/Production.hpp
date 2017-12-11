#pragma once

#include <queue>

#include "UnitManagement.hpp"
#include "Hashes.hpp"

namespace ZerGreenAI
{
	class ProductionManager : public UnitManager
	{
		std::unordered_map<BWAPI::UnitType, int> UnitWeights;
		std::unordered_map<BWAPI::UnitType, std::queue<BWAPI::UnitType>> ProduceLists;

		void cycleQueue(BWAPI::UnitType prod);
		void attemptTrain(BWAPI::Unit u);

	public:
		inline virtual std::string name() { return "Production Manager"; }
		void onStart() override;
		void onFrame() override;

		void SetUnitWeight(BWAPI::UnitType unit, int Weight);
		void UpdateWeightLists();
		BWAPI::Unitset getProductionBuildings();
	};
}