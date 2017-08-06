#pragma once

#include "Namespaces.h"
#include "UnitManagement.h"

class GlobalHarvestManager : public UnitManager
{
	void onAssignment(Unit u);
	void assignWorker(Unit u);
public:
	inline std::string name() { return "Global Harvester"; }

	void onFrame();

	int numHarvesters();
	int numMineralHarvesters();
	int numGasHarvesters();

	bool needProbes();
	Unit nearbyAvailableHarvester(Position p);
	TilePosition getAssimilatorPosition();
	TilePosition getBasePosition();
};

GlobalHarvestManager* getGlobalHarvester();
