#include "stdafx.h"

#include <BWAPI.h>
#include "ZerGreenAI.hpp"
#include "Construction.hpp"
#include "GlobalHarvesting.hpp"
#include "LayoutPlanner.hpp"
#include "Hashes.hpp"
#include "Debug.hpp"
#include "Timer.hpp"
#include "Namespaces.hpp"
#include "ResourceAllocator.hpp"

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
		giveUnitManagement(u, ZerGreenAIObj::mainInstance->resourceAllocator);
	}
}

bool ConstructionManager::constructBuilding(UnitType type)
{
	if (typeConstructionImminent[type])
	{
		return false;
	}

	TilePosition buildPos = ZerGreenAIObj::mainInstance->layoutPlanner->getAvailablePosition(type);
	if (buildPos == TilePositions::None)
	{
		return false;
	}

	Unit constructor = ZerGreenAIObj::mainInstance->globalHarvestManager->nearbyAvailableHarvester(Position(buildPos));

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