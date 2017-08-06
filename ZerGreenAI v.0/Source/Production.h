#pragma once

#include "UnitManagement.h"
#include "queue"
#include "Hashes.h"

class ProductionManager : public UnitManager
{
	std::unordered_map<UnitType, int> UnitWeights;
	std::unordered_map<UnitType, std::queue<UnitType>> ProduceLists;

	void cycleQueue(UnitType prod);
	void attemptTrain(Unit u);

public:
	inline std::string name() { return "Production Manager"; }
	void onFrame();

	void SetUnitWeight(UnitType unit, int Weight);
	void UpdateWeightLists();
	Unitset getProductionBuildings();
};

ProductionManager * getProductionManager();