#include "stdafx.h"

#include "GlobalHarvesting.hpp"
#include "LocalHarvesting.hpp"
#include "Timer.hpp"
#include "Hashes.hpp"
#include "bwemL.hpp"
#include "Namespaces.hpp"

void GlobalHarvestManager::assignWorker(Unit u)
{
	for (LocalHarvestManager * b : childManagers)
	{
		if (b->numHarvesters() < b->numSaturated())
		{
			giveUnitManagement(u, b);
			return;
		}
	}

	unsigned int leastWorkers = 100;
	LocalHarvestManager* bestManager = nullptr;
	for (const auto &b : childManagers)
	{
		if (b->numHarvesters() < leastWorkers)
		{
			leastWorkers = b->numHarvesters();
			bestManager = b;
		}
	}
	if (bestManager == nullptr)
	{
		return;
	}
	giveUnitManagement(u, bestManager);
}

void ZerGreenAI::GlobalHarvestManager::onUnitCreate(Unit u)
{
	if (IsResourceDepot(u))
	{
		unclaimedBases.erase(u->getTilePosition());
	}
}

void GlobalHarvestManager::onAssignment(Unit u)
{
	UnitManager::onAssignment(u);
	if (IsResourceDepot(u))
	{
		addHarvestManager(new LocalHarvestManager(u));
	}
	else if (IsWorker(u))
	{
		assignWorker(u);
	}
}

void GlobalHarvestManager::onFrame()
{
	UnitManager::onFrame();
	auto tempStorage = assignedUnits;
	for (auto const &u : tempStorage)
	{
		if (IsWorker(u))
		{
			assignWorker(u);
		}
	}
}

int GlobalHarvestManager::numHarvesters()
{
	return numMineralHarvesters() + numGasHarvesters();
}

int GlobalHarvestManager::numMineralHarvesters()
{
	int sum = 0;
	for (auto const &mgr : childManagers)
	{
		sum += mgr->numMineralHarvesters();
	}
	return sum;
}

int GlobalHarvestManager::numGasHarvesters()
{
	int sum = 0;
	for (auto const &mgr : childManagers)
	{
		sum += mgr->numGasHarvesters();
	}
	return sum;
}

Unit GlobalHarvestManager::giveNearbyAvailableHarvester(Position p, UnitManager * toWho)
{	
	double bestDistance = 100000;
	LocalHarvestManager * bestLocalHarvestManager = nullptr;
	for (auto const &mgr : childManagers)
	{
		if (mgr->numHarvesters() <= 0)
		{
			continue;
		}

		double testDistance = mgr->getPosition().getDistance(p);
		if (testDistance < bestDistance)
		{
			bestDistance = testDistance;
			bestLocalHarvestManager = mgr;
		}
	}

	if (bestLocalHarvestManager != nullptr)
	{
		return bestLocalHarvestManager->giveNearbyAvailableHarvester(p, toWho);
	}
	else
	{
		return nullptr;
	}

}

bool GlobalHarvestManager::needProbes()
{
	for (auto const &b : childManagers)
	{
		if (b->needProbes())
		{
			return true;
		}
	}
	return false;
}

TilePosition GlobalHarvestManager::getAssimilatorPosition()
{
	TilePosition tempPosition = TilePositions::None;
	for (auto const &mgr : childManagers)
	{
		tempPosition = mgr->getAssimilatorPosition();
		if (tempPosition != TilePositions::None)
		{
			return tempPosition;
		}
	}
	return tempPosition;
}

TilePosition GlobalHarvestManager::getBasePosition()
{
	TilePosition bestPosition = TilePositions::None;
	double bestDistance = DBL_MAX;
	TilePosition startPosition = Broodwar->self()->getStartLocation();
	for (auto const &b : unclaimedBases)
	{
		double currentDistance = b.getApproxDistance(startPosition);
		if (currentDistance < bestDistance)
		{
			bestDistance = b.getApproxDistance(startPosition);
			bestPosition = b;
		}
	}
	return bestPosition;
}

void GlobalHarvestManager::onStart()
{
	for (auto const &a : theMap.Areas())
	{
		for (auto const &b : a.Bases())
		{
			unclaimedBases.insert(b.Location());
		}
	}
}

ZerGreenAI::GlobalHarvestManager::~GlobalHarvestManager()
{
	for (auto const &m : childManagers)
	{
		delete m;
	}
}
