#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "CombatStrategist.hpp"
#include "Deathball.hpp"
#include "MacroCombatGroup.hpp"
#include "Namespaces.hpp"

//DeathballManager* MainDeathball;

void CombatStrategist::onAssignment(Unit u)
{
	giveUnitManagement(u, ZerGreenAIObj::mainInstance->macroCombatManager);
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