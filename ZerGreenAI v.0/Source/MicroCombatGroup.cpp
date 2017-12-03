#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "MicroCombatGroup.hpp"
#include "Vector.hpp"
#include "IMPScoutManager.hpp"
#include "Namespaces.hpp"
#include "TriangularGrid.hpp"

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

	target = (Position)ZerGreenAIObj::mainInstance->impScoutManager->getColdest();
	assignedUnits.attack(center);
	Broodwar->drawCircleMap(center, MCG_PUSH_RADIUS, Colors::Purple);
	Broodwar->drawCircleMap(center, MCG_LEASH, Colors::Purple);
	Broodwar->drawCircleMap(center, 2, Colors::Purple);
	Broodwar->drawCircleMap(target, 5, Colors::Orange);
	Broodwar->drawLineMap(center, target, Colors::Orange);

	if (center.getApproxDistance(assignedUnits.getPosition()) < MCG_PUSH_RADIUS)
	{
		Broodwar->drawCircleMap(ZerGreenAIObj::mainInstance->grid->snapToGrid(center), 5, Colors::Purple, true);
		Broodwar->drawCircleMap(ZerGreenAIObj::mainInstance->grid->snapToGrid(target), 5, Colors::Orange, true);

		std::vector<Position> path = ZerGreenAIObj::mainInstance->findPath(center, target);
		if (path.size() > 0)
		{
			center = path.front();
		}
	}
}

void MicroCombatManager::absorb(MicroCombatManager * other)
{
	assignedUnits.insert(other->assignedUnits.begin(), other->assignedUnits.end());
	other->assignedUnits.clear();
	delete other;
}
