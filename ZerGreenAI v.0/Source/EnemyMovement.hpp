#pragma once
#include "GeneralManagement.hpp"

namespace ZerGreenAI
{

	class EnemyMovementManager : Manager
	{
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