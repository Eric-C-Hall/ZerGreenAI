#include "stdafx.h"

#include "MicroCombatGroup.hpp"
#include "Vector.hpp"
#include "IMPScoutManager.hpp"
#include "Namespaces.hpp"

void MicroCombatManager::onUnitTurn(Unit u)
{
	u->move(center);
}

void MicroCombatManager::onFrame()
{
	if (assignedUnits.empty())
	{
		delete this;
		return;
	}

	target = (Position)getIMPScoutManager()->getColdest();
	assignedUnits.attack(center);
	Broodwar->drawCircleMap(center, MCG_PUSH_RADIUS, Colors::Purple);
	Broodwar->drawCircleMap(center, MCG_LEASH, Colors::Purple);
	Broodwar->drawCircleMap(center, 2, Colors::Purple);
	Broodwar->drawCircleMap(target, 5, Colors::Orange);
	Broodwar->drawLineMap(center, target, Colors::Orange);

	if (center.getApproxDistance(assignedUnits.getPosition()) < MCG_PUSH_RADIUS)
	{
		center += scaleVectorToSize(target - center, 100);
	}
}

void ZerGreenAI::MicroCombatManager::absorb(MicroCombatManager * other)
{
	assignedUnits.insert(other->assignedUnits.begin(), other->assignedUnits.end());
	other->assignedUnits.clear();
	delete other;
}
