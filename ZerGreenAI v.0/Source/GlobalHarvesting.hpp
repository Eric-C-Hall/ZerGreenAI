#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{
	class LocalHarvestManager;

	class GlobalHarvestManager : public UnitManager
	{
		void onAssignment(BWAPI::Unit u) override;
		void assignWorker(BWAPI::Unit u);

		std::unordered_set<LocalHarvestManager *> childManagers;
		std::unordered_set<TilePosition> unclaimedBases;

		inline void addHarvestManager(LocalHarvestManager * mgr) { childManagers.insert(mgr); }
		void onStart() override;
		void onUnitCreate(Unit u) override;
	public:
		~GlobalHarvestManager();

		inline virtual std::string name() { return "Global Harvester"; }

		void onFrame() override;

		int numHarvesters();
		int numMineralHarvesters();
		int numGasHarvesters();
		inline int numClaimedBases() { return childManagers.size(); }

		bool needProbes();
		BWAPI::Unit giveNearbyAvailableHarvester(BWAPI::Position p, UnitManager * giveToWho);
		BWAPI::TilePosition getAssimilatorPosition();
		BWAPI::TilePosition getBasePosition();
	};

}