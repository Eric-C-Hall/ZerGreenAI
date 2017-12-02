#include "stdafx.h"

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

void ZerGreenAI::MacroCombatManager::onFrame()
{
	static bool hasOccurred = false;
	for (MicroCombatManager * c1 : childManagers)
	{
		for (MicroCombatManager * c2 : childManagers)
		{
			
			if (c1 == c2 || cleanUpList.count(c2) != 0)
			{
				continue;
			}
			if (c1->getCenter().getApproxDistance(c2->getCenter()) < MCG_LEASH)
			{
				hasOccurred = true;
				c1->absorb(c2);
				return;
			}
		}
	}


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