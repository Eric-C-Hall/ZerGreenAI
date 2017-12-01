#include "MacroCombatGroup.hpp"
#include "MicroCombatGroup.hpp"
#include "Production.hpp"
#include "Namespaces.hpp"

MacroCombatManager localMacroCombatManager;

void MacroCombatManager::newManager(Unit u)
{
	MicroCombatManager * newManager = new MicroCombatManager(u->getPosition(), u->getPosition());
	childManagers.insert(newManager);
	giveUnitManagement(u, newManager);
}

void MacroCombatManager::onAssignment(Unit u)
{
	if (childManagers.size() > 0)
	{
		MicroCombatManager * bestManager;
		double bestDistance = DBL_MAX;
		for (MicroCombatManager * currentManager : childManagers)
		{
			double currentDistance = u->getDistance(((MicroCombatManager *)currentManager)->getCenter());
			if (currentDistance < bestDistance)
			{
				bestManager = currentManager;
				bestDistance = currentDistance;
			}
		}

		std::cout << bestDistance << std::endl;
		if (bestDistance < MCG_LEASH)
		{
			giveUnitManagement(u, bestManager);
		}
		else
		{
			newManager(u);
		}
			
	}
	else
	{
		newManager(u);
	}
}

MacroCombatManager * ZerGreenAI::getMacroCombatManager()
{
	return &localMacroCombatManager;
}