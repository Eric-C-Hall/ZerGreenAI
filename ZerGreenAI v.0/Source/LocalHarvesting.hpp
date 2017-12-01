#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class LocalHarvestManager : public UnitManager
	{
		BWAPI::Unit base;
		BWAPI::Unitset nearbyMinerals;
		BWAPI::Unitset nearbyGeysers;
		BWAPI::Unitset nearbyResources;
		std::unordered_map<BWAPI::Unit, BWAPI::Unit> Gatherer2Mineral;
		std::unordered_map<BWAPI::Unit, BWAPI::Unitset> Mineral2Gatherers;

		void createPair(BWAPI::Unit gath, BWAPI::Unit min);
		void erasePair(BWAPI::Unit gath);

		void onAssignment(BWAPI::Unit u);
		bool acceptRequest(BWAPI::Unit u); // Preferably should blacklist rather than whitelist
		void recycleUnit(BWAPI::Unit u);

		void drawDebugResourceInfo();

	public:
		inline std::string name() { return "Local Harvester"; }

		unsigned int numSaturated();
		unsigned int numHarvesters();
		unsigned int numMineralHarvesters();
		unsigned int numGasHarvesters();
		BWAPI::Position getPosition();
		bool needProbes();
		BWAPI::Unit nearbyAvailableHarvester(BWAPI::Position p);
		BWAPI::TilePosition getAssimilatorPosition();
		LocalHarvestManager(BWAPI::Unit u);
		void onFrame();

		// if the following ends up being deleted, delete in cpp too.
		//void reassessOldWorkers();
	};

}