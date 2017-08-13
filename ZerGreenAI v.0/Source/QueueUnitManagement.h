#pragma once

#include "UnitManagement.h"
#include <queue>

class QueueUnitManager : protected UnitManager
{
	std::queue<Unit> unitDecideQueue;
	Unit rotateQueue();
	void onAssignment(Unit u);
	virtual void onUnitTurn(Unit u) {};
	void onFrame();
public:
	inline virtual std::string name() { return "Unnamed Queue Unit Manager"; }
};