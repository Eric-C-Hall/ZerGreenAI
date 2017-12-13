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
#include "ProbeScout.hpp"

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

void ConstructionManager::onFrame()
{
	UnitManager::onFrame();
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	for (auto const &u : assignedUnits)
	{
		TilePosition topLeft = buildPosition[u];
		TilePosition bottomRight = buildPosition[u] + buildType[u].tileSize();
		new debugBox(CoordinateType::Map, (Position)topLeft, (Position)bottomRight, Colors::Red, Broodwar->getLatencyFrames(), false);
		new debugText((Position)topLeft + Position(10, 10), buildType[u].c_str(), Broodwar->getLatencyFrames());

		if (!Broodwar->isVisible(topLeft) || !Broodwar->isVisible(bottomRight))
		{
			u->move(Position(topLeft));
		}
		else if (u->canBuild(buildType[u], buildPosition[u]))
		{
			u->build(buildType[u], buildPosition[u]);
		}
		else if (!Broodwar->canBuildHere(buildPosition[u], buildType[u]))
		{
			buildPosition[u] = ZerGreenAIObj::mainInstance->layoutPlanner->getAvailablePosition(buildType[u]);
		}
		else if (u->getOrder() != Orders::PlaceBuilding)
		{
			u->move(Position(topLeft));
		}

		new debugText(CoordinateType::Map, u->getPosition().x, u->getPosition().y, "Faster, Lazy Pleb", Broodwar->getLatencyFrames());
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

				static bool firstPylon = true;
				if (firstPylon && u->getType() == UnitTypes::Protoss_Pylon)
				{
					startProbeScout();
					firstPylon = false;
				}
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
	// Might be wrong morph direction
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

	if (!Broodwar->canMake(type))
	{
		return false;
	}
		
	TilePosition buildPos = ZerGreenAIObj::mainInstance->layoutPlanner->getAvailablePosition(type);
	if (buildPos == TilePositions::None)
	{
		return false;
	}

	Unit constructor = ZerGreenAIObj::mainInstance->globalHarvestManager->giveNearbyAvailableHarvester(Position(buildPos), this);

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

	buildType[constructor] = type;
	buildPosition[constructor] = buildPos;
	if (!constructor->build(type, buildPos))
	{
		constructor->move(Position(buildPos));
	}
	typeConstructionImminent[type] = true;

	return true;
}