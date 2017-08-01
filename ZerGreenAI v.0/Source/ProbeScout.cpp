#include "ProbeScout.h"
#include "GlobalHarvesting.h"
#include "Hashes.h"
#include "ScoutAnalysis.h"

std::unordered_set<TilePosition> seenLocations;
Unit probe;

void ProbeScoutManager::recycleUnit(Unit u)
{
	deleteManagerWhenPossible(this);
}

TilePosition ProbeScoutManager::enemyBase = TilePositions::Unknown;

ProbeScoutManager::ProbeScoutManager(Unit u)
{
	probe = u;
	if (!requestUnitManagement(u))
	{
		Broodwar << "Error: ProbeScoutManager given probe without consent" << std::endl;
	}
}

Unit gasToSteal;

void stealGas(Unit u)
{
	gasToSteal = u;
}

void ProbeScoutManager::onFrame()
{
	if (enemyBase != TilePositions::Unknown)
	{
		if (gasToSteal != nullptr)
		{
			if (!Broodwar->isVisible(gasToSteal->getTilePosition()))
			{
				probe->move(gasToSteal->getPosition());
			}
			else if (!probe->isConstructing())
			{
				probe->build(UnitTypes::Protoss_Assimilator, gasToSteal->getTilePosition());
			}
		}
		else
		{
			
		}
	}
	else
	{
		for (auto const &b : Broodwar->getStartLocations())
		{

			if (!Broodwar->isVisible(b) && seenLocations.count(b) < 1)
			{
				probe->move(Position(b));
				break;
			}
			else
			{
				seenLocations.insert(b);
			}
		}
	}
}

void startProbeScout()
{
	addManager(new ProbeScoutManager(getGlobalHarvester()->nearbyAvailableHarvester(Position(Broodwar->getStartLocations().front()))));
}

void ProbeScoutManager::foundBase(TilePosition t)
{
	enemyBase = t;
}