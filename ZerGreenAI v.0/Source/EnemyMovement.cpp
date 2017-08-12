#include "EnemyMovement.h"
#include "bwStrings.h"

EnemyMovementManager enemyMovementManager;

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

EnemyMovementManager * getEnemyMovementManager()
{
	return &enemyMovementManager;
}