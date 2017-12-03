#pragma once
#include "GeneralManagement.hpp"

namespace ZerGreenAI
{

	class EnemyMovementManager : Manager
	{
		void onUnitDiscover(BWAPI::Unit u);
		void onFrame();
		void onUnitDestroy(BWAPI::Unit u);
		inline virtual std::string name() { return "Enemy Movement Manager"; }

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