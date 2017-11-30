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
#include "QueueUnitManagement.h"

class LearningDeathballManager : public QueueUnitManager
{
	void onUnitTurn(Unit u);
	void onAssignmentQ(Unit u);
	void recycleUnit(Unit u);
public:
	inline virtual std::string name() { return "Deathball Manager Mk 2"; }
};