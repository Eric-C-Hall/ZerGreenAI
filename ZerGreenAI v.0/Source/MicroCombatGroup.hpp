#pragma once

#include "QueueUnitManagement.h"

class MicroCombatManager : public QueueUnitManager
{
	void onUnitTurn(Unit u);
public:
	inline virtual std::string name() { return "Micro Combat Manager"; }
};