#pragma once

#include "Namespaces.h"
#include "UnitManagement.h"

class CombatStrategist : public UnitManager
{
	void onStart();
	void onAssignment(Unit u);
public:
	inline virtual std::string name() { return "Combat Strategist"; }
	void onFrame();
};

CombatStrategist * getCombatStrategist();