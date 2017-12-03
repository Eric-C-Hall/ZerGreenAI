#pragma once

#include "UnitManagement.hpp"
#include <queue>

namespace ZerGreenAI
{
	class DeathballManager : public UnitManager
	{
		BWAPI::Position currentBallPosition;
		BWAPI::Position goal;
		bool wouldWinConfrontation;
		void onAssignment(BWAPI::Unit u);
		void recycleUnit(BWAPI::Unit u);
		void unitDecide(BWAPI::Unit u);
		void goDirect(BWAPI::Unit u);
		bool isTogether();
		BWAPI::Unitset getNearbyEnemies();
		BWAPI::Unitset getNearbyUnits();
		std::queue<BWAPI::Unit> unitDecideQueue;
		BWAPI::Unit rotateQueue();
		void updateBallPosition();
		void updateGoal();
		void updateConfrontation();

	public:
		void onFrame();
		inline virtual std::string name() { return "Deathball Manager"; }
		DeathballManager();
	};

}