#pragma once

#include "Namespaces.h"
#include "Management.h"

class CombatStrategist : public JuniorManager
{
	void onAssignment(Unit u);
public:
	inline virtual std::string name() { return "Combat Strategist"; }
	void onFrame();
};

void initializeCombatStrategist();
CombatStrategist* getCombatStrategist();