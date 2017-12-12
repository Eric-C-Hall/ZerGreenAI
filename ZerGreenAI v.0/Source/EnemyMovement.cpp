#include "stdafx.h"

#include "EnemyMovement.hpp"
#include "Namespaces.hpp"
#include "bwStrings.hpp"

UnitTracker::UnitTracker(Unit u)
{
	position = u->getPosition();
	type = u->getType();
	id = u->getID();
	whatUnit = u;
}

void UnitTracker::update()
{
	if (whatUnit->exists())
	{
		position = whatUnit->getPosition();
	}
}

void UnitTracker::draw()
{
	Broodwar->drawTextMap(position, "%d: %s", id, noPrefixC_str(type));
}

namespace std
{
	template<>
	struct hash<UnitTracker>
	{
		size_t operator()(UnitTracker datum)
		{
			return datum.id;
		}
	};
}

std::unordered_map<int, UnitTracker *> trackedUnits;

void EnemyMovementManager::onUnitDiscover(Unit u)
{
	if (IsEnemy(u))
	{
		if (trackedUnits[u->getID()] == nullptr)
		{
			trackedUnits[u->getID()] = new UnitTracker(u);
		}
	}
}

void EnemyMovementManager::onFrame()
{
	for (auto const &t : trackedUnits)
	{
		t.second->update();
		t.second->draw();
	}
}

void EnemyMovementManager::onUnitDestroy(Unit u)
{
	trackedUnits.erase(u->getID());
}

void ZerGreenAI::EnemyMovementManager::onUnitMorph(BWAPI::Unit u)
{
	if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
	{
		trackedUnits.erase(u->getID());
	}
}

BWAPI::Position ZerGreenAI::EnemyMovementManager::getNearestTrackedEnemyLocation(BWAPI::Position p)
{
	double bestDistance = DBL_MAX;
	Position bestPos = Positions::None;
	for (auto const &t : trackedUnits)
	{
		Position currentPos = t.second->position;
		double currentDistance = currentPos.getApproxDistance(p);
		if (currentDistance <= bestDistance)
		{
			bestDistance = currentDistance;
			bestPos = currentPos;
		}
	}
	return bestPos;
}
