#include "CombatStrategist.h"
#include "Deathball.h"

CombatStrategist* LocalCombatStrategist;
DeathballManager* MainDeathball;

void CombatStrategist::onAssignment(Unit u)
{
	giveUnitManagement(u, MainDeathball);
}

void CombatStrategist::onFrame()
{
	for (auto const &u : assignedUnits)
	{
		onAssignment(u);
	}
}

void initializeCombatStrategist()
{
	LocalCombatStrategist = new CombatStrategist;
	addManager(LocalCombatStrategist);
	MainDeathball = new DeathballManager;
	addManager(MainDeathball);
}

CombatStrategist* getCombatStrategist()
{
	return LocalCombatStrategist;
}