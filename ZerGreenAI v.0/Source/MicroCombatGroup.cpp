#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "MacroCombatGroup.hpp"
#include "MicroCombatGroup.hpp"
#include "Vector.hpp"
#include "IMPScoutManager.hpp"
#include "Namespaces.hpp"
#include "TriangularGrid.hpp"
#include "Debug.hpp"

void MicroCombatManager::onUnitTurn(Unit u)
{
	debugUnitText(u, "hasHadATurn");

	Unitset neutralUnits = u->getUnitsInRadius(MCG_PUSH_RADIUS, IsNeutral);
	if (neutralUnits.size() > 0)
	{
		u->attack(*neutralUnits.begin());
	}
	else
	{
		u->attack(center);
	}
}

void ZerGreenAI::MicroCombatManager::onReassignment(BWAPI::Unit u)
{
	QueueUnitManager::onReassignment(u);
	if (assignedUnits.size() == 0)
		delete this;
}

void ZerGreenAI::MicroCombatManager::updateTarget(BWAPI::Position p)
{
	if (target != p)
	{
		target = p;
		path = ZerGreenAIObj::mainInstance->findPath(center, target);
		for (Unit const &u : assignedUnits)
			u->attack(center);
	}
}

ZerGreenAI::MicroCombatManager::~MicroCombatManager()
{
	ZerGreenAIObj::mainInstance->macroCombatManager->childManagers.erase(this);
}

void MicroCombatManager::onFrame()
{
	QueueUnitManager::onFrame();
	if (assignedUnits.empty())
	{
		delete this;
		return;
	}

	for (const Position &p : path)
	{
		Broodwar->drawCircleMap(p, 5, Colors::Green, true);
	}

	Broodwar->drawCircleMap(ZerGreenAIObj::mainInstance->grid->snapToGrid(center), 4, Colors::Purple, true);
	Broodwar->drawCircleMap(ZerGreenAIObj::mainInstance->grid->snapToGrid(target), 4, Colors::Orange, true);

	Broodwar->drawCircleMap(center, MCG_PUSH_RADIUS, Colors::Purple);
	Broodwar->drawCircleMap(center, MCG_LEASH, Colors::Purple);
	Broodwar->drawLineMap(center, target, Colors::Orange);

	if (center.getApproxDistance(assignedUnits.getPosition()) < MCG_PUSH_RADIUS)
	{
		if (path.size() > 0)
		{
			center = path.front();
			path.erase(path.begin());
		}
	}
}

void MicroCombatManager::absorb(MicroCombatManager * other)
{
	assignedUnits.insert(other->assignedUnits.begin(), other->assignedUnits.end());
	other->assignedUnits.clear();
	delete other;
}
