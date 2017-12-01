#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class GlobalHarvestManager : public UnitManager
	{
		void onAssignment(BWAPI::Unit u);
		void assignWorker(BWAPI::Unit u);
	public:
		inline std::string name() { return "Global Harvester"; }

		void onFrame();

		int numHarvesters();
		int numMineralHarvesters();
		int numGasHarvesters();

		bool needProbes();
		BWAPI::Unit nearbyAvailableHarvester(BWAPI::Position p);
		BWAPI::TilePosition getAssimilatorPosition();
		BWAPI::TilePosition getBasePosition();
	};

	GlobalHarvestManager* getGlobalHarvester();

}