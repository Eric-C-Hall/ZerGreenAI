#include "stdafx.h"

#include "CombatStrategist.hpp"
#include "Deathball.hpp"
#include "MacroCombatGroup.hpp"
#include "Namespaces.hpp"

CombatStrategist LocalCombatStrategist;
//DeathballManager* MainDeathball;

void CombatStrategist::onAssignment(Unit u)
{
	giveUnitManagement(u, getMacroCombatManager());
}

void CombatStrategist::onFrame()
{
	for (auto const &u : assignedUnits)
	{
		onAssignment(u);
	}
}

void CombatStrategist::onStart()
{
	//MainDeathball = new DeathballManager;
}

CombatStrategist* ZerGreenAI::getCombatStrategist()
{
	return &LocalCombatStrategist;
}