#include "Construction.h"
#include "GlobalHarvesting.h"
#include "LayoutPlanner.h"
#include "Hashes.h"
#include "Debug.h"
#include "Timer.h"

ConstructionManager LocalConstructionManager;

std::unordered_map<Unit, UnitType> buildType;
std::unordered_map<Unit, TilePosition> buildPosition;
std::unordered_map<Unit, int> newWorkerLeeway;

std::unordered_map<UnitType, bool> typeConstructionImminent;

bool ConstructionManager::acceptRequest(Unit u)
{
	if (u->isConstructing())
	{
		return false;
	}
	typeConstructionImminent[buildType[u]] = false;
	buildType.erase(u);
	buildPosition.erase(u);
	return true;
}

void ConstructionManager::recycleUnit(Unit u)
{

}

void ConstructionManager::onFrame()
{
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	Unitset unAssignedUnits;

	for (auto const &u : assignedUnits)
	{
		TilePosition topLeft = buildPosition[u];
		TilePosition bottomRight = buildPosition[u] + buildType[u].tileSize();
		new debugBox(CoordinateType::Map, topLeft.x, bottomRight.x, topLeft.y, bottomRight.y, Colors::Red, Broodwar->getLatencyFrames());

		if (!Broodwar->isVisible(topLeft) || !Broodwar->isVisible(bottomRight))
		{
			u->move(Position(topLeft));
		}
		else if (u->canBuild(buildType[u], buildPosition[u]))
		{
			u->build(buildType[u], buildPosition[u]);
		}
		else if (!u->isConstructing() && newWorkerLeeway[u] <= 0)
		{
			typeConstructionImminent[buildType[u]] = false;
			buildType.erase(u);
			buildPosition.erase(u);
			unAssignedUnits.insert(u);
		}
		else
		{
			newWorkerLeeway[u]--;
			new debugText(CoordinateType::Map, u->getPosition().x, u->getPosition().y, "Faster, Lazy Pleb", Broodwar->getLatencyFrames());
		}

	}

	for (auto const &u : unAssignedUnits)
	{
		giveUnitManagement(u, getResourceAllocator());
	}
}

bool ConstructionManager::constructBuilding(UnitType type)
{
	if (typeConstructionImminent[type])
	{
		return false;
	}

	TilePosition buildPos = getLayoutPlanner()->getAvailablePosition(type);
	if (buildPos == TilePositions::None)
	{
		return false;
	}

	Unit constructor = getGlobalHarvester()->nearbyAvailableHarvester(Position(buildPos));

	if (constructor == nullptr)
	{
		return false;
	}

	if (!Broodwar->canBuildHere(buildPos, type, constructor))
	{
		Error err = Broodwar->getLastError();
		Broodwar << "Constructor cannot build: " << err.c_str() << std::endl;
		return false;
	}

	requestUnitManagement(constructor);
	buildType[constructor] = type;
	buildPosition[constructor] = buildPos;
	newWorkerLeeway[constructor] = 10 + Broodwar->getLatencyFrames();
	if (!constructor->build(type, buildPos))
	{
		constructor->move(Position(buildPos));
	}
	typeConstructionImminent[type] = true;

	return true;
}

ConstructionManager * getConstructionManager()
{
	return &LocalConstructionManager;
}