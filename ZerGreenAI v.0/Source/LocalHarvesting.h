#pragma once

#include "Namespaces.h"
#include "Management.h"

class LocalHarvestManager : public JuniorManager
{
	Unit base;
	Unitset nearbyMinerals;
	Unitset nearbyGeysers;
	Unitset nearbyResources;
	std::unordered_map<Unit, Unit> Gatherer2Mineral;
	std::unordered_map<Unit, Unitset> Mineral2Gatherers;

	void createPair(Unit gath, Unit min);
	void erasePair(Unit gath);

	void onAssignment(Unit u);
	bool acceptRequest(Unit u); // Preferably should blacklist rather than whitelist
	void recycleUnit(Unit u);

	void drawDebugResourceInfo();

public:
	inline std::string name() { return "Local Harvester";}

	int numSaturated();
	int numHarvesters();
	int numMineralHarvesters();
	int numGasHarvesters();
	Position getPosition();
	Unit nearbyAvailableHarvester(Position p);
	TilePosition getAssimilatorPosition();
	LocalHarvestManager(Unit u);
	void onFrame();

	// if the following ends up being deleted, delete in cpp too.
	//void reassessOldWorkers();
};
