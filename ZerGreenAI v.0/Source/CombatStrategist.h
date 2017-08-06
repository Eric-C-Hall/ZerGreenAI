#pragma once

#include "Namespaces.h"
#include "Management.h"

class CombatStrategist : public JuniorManager
{
	void onStart();
	void onAssignment(Unit u);
public:
	inline virtual std::string name() { return "Combat Strategist"; }
	void onFrame();
};

CombatStrategist * getCombatStrategist();