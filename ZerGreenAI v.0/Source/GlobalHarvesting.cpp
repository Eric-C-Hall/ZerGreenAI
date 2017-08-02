#include "GlobalHarvesting.h"
#include "LocalHarvesting.h"
#include "Timer.h"
#include "Hashes.h"
#include "bwemL.h"

std::unordered_set<LocalHarvestManager *> childManagers;
std::unordered_set<TilePosition> unclaimedBases;

GlobalHarvestManager* LocalGlobalHarvester;

void addHarvestManager(LocalHarvestManager * mgr)
{
	childManagers.insert(mgr);
	addManager(mgr);
}

void GlobalHarvestManager::assignWorker(Unit u)
{
	for (const auto &b : childManagers)
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

void GlobalHarvestManager::onAssignment(Unit u)
{
	if (IsResourceDepot(u))
	{
		addHarvestManager(new LocalHarvestManager(u));
		unclaimedBases.erase(u->getTilePosition());
	}
	else if (IsWorker(u))
	{
		assignWorker(u);
	}
}

void GlobalHarvestManager::onFrame()
{
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

Unit GlobalHarvestManager::nearbyAvailableHarvester(Position p)
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
		return bestLocalHarvestManager->nearbyAvailableHarvester(p);
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
	double bestDistance = 100000;
	TilePosition startPosition = Broodwar->self()->getStartLocation();
	for (auto const &b : unclaimedBases)
	{
		if (b.getApproxDistance(startPosition) < bestDistance)
		{
			bestDistance = b.getApproxDistance(startPosition);
			bestPosition = b;
		}
	}
	return bestPosition;
}

void initializeGlobalHarvester()
{
	LocalGlobalHarvester = new GlobalHarvestManager;
	addManager(LocalGlobalHarvester);

	for (auto const &a : theMap.Areas())
	{
		for (auto const &b : a.Bases())
		{
			unclaimedBases.insert(b.Location());
		}
	}
}

GlobalHarvestManager* getGlobalHarvester()
{
	return LocalGlobalHarvester;
}