#pragma once

#include "Namespaces.h"
#include "Management.h"
#include <queue>

class DeathballManager : public JuniorManager
{
	Position currentBallPosition;
	Position goal;
	bool wouldWinConfrontation;
	void onAssignment(Unit u);
	void recycleUnit(Unit u);
	void unitDecide(Unit u);
	void goDirect(Unit u);
	bool isTogether();
	Unitset getNearbyUnits();
	std::queue<Unit> unitDecideQueue;
	Unit rotateQueue();
	void updateBallPosition();
	void updateGoal();
	void updateConfrontation();

public:
	void onFrame();
	inline virtual std::string name() { return "Deathball Manager"; }
	DeathballManager();
};