#pragma once

#include "Namespaces.h"
#include "Management.h"

class GlobalHarvestManager : public JuniorManager
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

void initializeGlobalHarvester();
GlobalHarvestManager* getGlobalHarvester();
