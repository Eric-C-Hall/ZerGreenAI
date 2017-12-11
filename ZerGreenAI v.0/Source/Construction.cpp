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

	for (auto const &u : assignedUnits)
	{
		TilePosition topLeft = buildPosition[u];
		TilePosition bottomRight = buildPosition[u] + buildType[u].tileSize();
		new debugBox(CoordinateType::Map, (Position)topLeft, (Position)bottomRight, Colors::Red, Broodwar->getLatencyFrames(), false);

		if (!Broodwar->isVisible(topLeft) || !Broodwar->isVisible(bottomRight))
		{
			u->move(Position(topLeft));
		}
		else if (u->canBuild(buildType[u], buildPosition[u]))
		{
			u->build(buildType[u], buildPosition[u]);
		}
		else
		{
			new debugText(CoordinateType::Map, u->getPosition().x, u->getPosition().y, "Faster, Lazy Pleb", Broodwar->getLatencyFrames());
		}

	}
}

void ZerGreenAI::ConstructionManager::onUnitCreate(Unit u)
{
	if ((IsBuilding && IsOwned)(u))
	{
		Unitset finishedWorkers;
		for (auto const &currWorkerPosPair : buildPosition)
		{
			if (u->getTilePosition() == currWorkerPosPair.second)
			{
				finishedWorkers.insert(currWorkerPosPair.first);
			}
		}
		for (auto const &u : finishedWorkers)
		{
			typeConstructionImminent[buildType[u]] = false;
			buildType.erase(u);
			buildPosition.erase(u);
			giveUnitManagement(u, ZerGreenAIObj::mainInstance->resourceAllocator);
		}
	}
}

void ZerGreenAI::ConstructionManager::onUnitMorph(Unit u)
{
	if ((IsOwned && IsRefinery)(u))
	{
		onUnitCreate(u);
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
	if (!constructor->build(type, buildPos))
	{
		constructor->move(Position(buildPos));
	}
	typeConstructionImminent[type] = true;

	return true;
}