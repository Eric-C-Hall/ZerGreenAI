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
	center = assignedUnits.getPosition();
	target = (Position)getIMPScoutManager()->getColdest();
	if ((*assignedUnits.begin())->getTargetPosition() != target)
		assignedUnits.attack(target);
	Broodwar->drawCircleMap(center, MCG_LEASH, Colors::Purple);
	Broodwar->drawCircleMap(center, 2, Colors::Purple);
	Broodwar->drawCircleMap(target, 5, Colors::Orange);
	Broodwar->drawLineMap(center, target, Colors::Orange);

	if (center.getApproxDistance(assignedUnits.getPosition()) < MCG_PUSH_RADIUS)
	{
		//center = center + unitVector(center - target);
	}
}
