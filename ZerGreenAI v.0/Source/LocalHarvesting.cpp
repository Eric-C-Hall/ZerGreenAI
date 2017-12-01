#include "LocalHarvesting.hpp"
#include "GlobalHarvesting.hpp"
#include "Debug.hpp"
#include "bwemL.hpp"
#include "Namespaces.hpp"

void LocalHarvestManager::createPair(Unit gath, Unit min)
{
	erasePair(gath);
	Gatherer2Mineral[gath] = min;
	Mineral2Gatherers[min].insert(gath);
}

void LocalHarvestManager::erasePair(Unit gath)
{
	if (Gatherer2Mineral[gath] == nullptr)
	{
		Gatherer2Mineral.erase(gath);
		return;
	}
	Mineral2Gatherers[Gatherer2Mineral[gath]].erase(gath);
	Gatherer2Mineral.erase(gath);
}

void LocalHarvestManager::onAssignment(Unit u)
{
	for (auto const &g : nearbyGeysers)
	{
		if ((IsRefinery && IsCompleted)(g) && Mineral2Gatherers[g].size() < 3)
		{
			createPair(u, g);
			return;
		}
	}
	unsigned int lowestNumberMining = 100;
	Unit bestMineral = nullptr;
	for (auto const &m : nearbyMinerals)
	{
		if (Mineral2Gatherers[m].size() < lowestNumberMining)
		{
			lowestNumberMining = Mineral2Gatherers[m].size();
			bestMineral = m;
		}
	}
	if (lowestNumberMining < 2)
	{
		createPair(u, bestMineral);
	}
	else
	{
		giveUnitManagement(u, getResourceAllocator());
	}

}

bool LocalHarvestManager::acceptRequest(Unit u)
{
	erasePair(u);
	return true;
}

void LocalHarvestManager::recycleUnit(Unit u)
{
	if (u == base)
	{
		delete this;
	}
	else
	{
		erasePair(u);
	}
}

int maxGatherers(Unit m)
{
	if (IsMineralField(m))
	{
		return 2;
	}
	else if (IsResourceContainer(m))
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

void LocalHarvestManager::drawDebugResourceInfo()
{
	for (auto const &m : nearbyResources)
	{
		Broodwar->drawTextMap(m->getPosition(), "%d/%d", Mineral2Gatherers[m].size(), maxGatherers(m));
	}
}

#define MINERAL_UMBILICAL 1
void LocalHarvestManager::onFrame()
{
	drawDebugResourceInfo();

	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	for (auto const &u : assignedUnits)
	{
		if (Gatherer2Mineral[u] != nullptr)
		{
#if MINERAL_UMBILICAL && _DEBUG
			new debugLine(CoordinateType::Map, u->getPosition(), Gatherer2Mineral[u]->getPosition(), Colors::Blue, Broodwar->getLatencyFrames());
#endif
		}
	}

	for (auto const &r : nearbyResources)
	{
		for (auto const &gath : Mineral2Gatherers[r])
		{
#ifdef _DEBUG
			if (gath->isSelected())
			{
				continue;
			}
#endif

			if ((IsGatheringGas || IsGatheringMinerals)(gath))
			{
				Unit targetUnit = gath->getOrderTarget();
				if ( (targetUnit == nullptr || !targetUnit->exists())  ||  (targetUnit != r && IsResourceContainer(targetUnit)))
				{
					gath->gather(r);
				}
			}
			else
			{
				gath->gather(r);
			}
		}
	}

	if (getGlobalHarvester()->needProbes())
	{
		if (base->getRemainingTrainTime() - 24 < 0)
		{
			if (base->getTrainingQueue().size() < 2)
			{
				base->train(UnitTypes::Protoss_Probe);
			}
		}
		else if (base->getTrainingQueue().size() > 1)
		{
			base->cancelTrain();
		}
	}
}

unsigned int LocalHarvestManager::numSaturated()
{
	return 3 * nearbyGeysers.size() + 2 * nearbyMinerals.size();
}

unsigned int LocalHarvestManager::numGasHarvesters()
{
	int temp = 0;
	for (auto const &g : nearbyGeysers)
	{
		for (auto const &u : Mineral2Gatherers[g])
		{
			temp++;
		}
	}
	return temp;
}

unsigned int LocalHarvestManager::numMineralHarvesters()
{
	return numHarvesters() - numGasHarvesters();
}

unsigned int LocalHarvestManager::numHarvesters()
{
	return assignedUnits.size();
}

Position LocalHarvestManager::getPosition()
{
	return base->getPosition();
}

bool LocalHarvestManager::needProbes()
{
	return assignedUnits.size() < numSaturated();
}

Unit LocalHarvestManager::nearbyAvailableHarvester(Position p)
{
	double bestDistance = 100000;
	Unit bestUnit;
	for (auto const &u : assignedUnits)
	{
		double testDistance = u->getPosition().getDistance(p);
		if (testDistance < bestDistance)
		{
			bestDistance = testDistance;
			bestUnit = u;
		}
	}
	return bestUnit;
}

TilePosition LocalHarvestManager::getAssimilatorPosition()
{
	for (auto const &g : nearbyGeysers)
	{
		if (g->getType() == UnitTypes::Resource_Vespene_Geyser)
		{
			return g->getTilePosition();
		}
	}
	return TilePositions::None;
}

LocalHarvestManager::LocalHarvestManager(Unit u)
{
	assert(!theMap.GetNearestArea(u->getTilePosition())->Bases().empty());
	const BWEM::Base * temp_base = &theMap.GetNearestArea(u->getTilePosition())->Bases().front();

	base = u;

	for (auto const &m : temp_base->Minerals())
	{
		nearbyMinerals.insert(m->Unit());
		nearbyResources.insert(m->Unit());
	}

	for (auto const &g : temp_base->Geysers())
	{
		nearbyGeysers.insert(g->Unit());
		nearbyResources.insert(g->Unit());
	}

	Broodwar->pingMinimap(u->getPosition());
}

/*void LocalHarvestManager::reassessOldWorkers()
{
	for (auto const &r : nearbyResources)
	{
		if (Mineral2Gatherers[r].size() > maxGatherers(r))
		{
			for (auto const &u : Mineral2Gatherers[r])
			{
				if (!u->isGa)
				{
					// TODO: Reassess worker
					break;
				}
			}
		}
	}
}*/