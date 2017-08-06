#pragma once

// PEAS
//
// Performance:
//
// The difference between the amount of minerals/gas lost by the enemy compared to the amount of minerals/gas lost by allies.
//
// Environment:
//
// Possible blockages, high ground, units.
//
// Actions: 
//
// Right-Click(location), Attack(enemy), Tech(location)/Tech(enemy)
//
// Sensors
//
// 
//

#include "Namespaces.h"
#include "UnitManagement.h"

class DeathballManager : public UnitManager
{
	Position goal;
	Position start;
public:
	void onFrame();
	inline virtual std::string name() { return "Deathball Manager Mk 2"; }
};