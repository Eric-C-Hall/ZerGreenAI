#pragma once
#include "GeneralManagement.hpp"

namespace ZerGreenAI
{
	class UnitTracker;

	class EnemyMovementManager : Manager
	{
		static const int FRAMES_VISIBLE_TO_DECIDE_GONE = 10;

		std::unordered_map<int, UnitTracker *> trackedUnits;
		std::unordered_map<UnitTracker *, int> visibleTrackers;

		void onUnitDiscover(BWAPI::Unit u) override;
		void onFrame() override;
		void onUnitDestroy(BWAPI::Unit u) override;
		void onUnitMorph(BWAPI::Unit u) override;
		inline virtual std::string name() { return "Enemy Movement Manager"; }

	public:
		BWAPI::Position getNearestTrackedEnemyLocation(BWAPI::Position p);

	};

	class UnitTracker
	{
		BWAPI::Unit whatUnit;
		void update();
		void draw();

	public:
		BWAPI::UnitType type;
		int id;
		BWAPI::Position position;

		UnitTracker(BWAPI::Unit u);
		friend EnemyMovementManager;
	};
}