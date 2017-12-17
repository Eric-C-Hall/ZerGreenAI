#include "stdafx.h"

#include <algorithm>

#include "Production.hpp"
#include "Debug.hpp"
#include "Namespaces.hpp"

void ZerGreenAI::ProductionManager::onFrame()
{
	auto tiedDownBuildingTypesCopy = tiedDownBuildingTypes;
	for (auto typeIntPair : tiedDownBuildingTypesCopy)
	{
		tiedDownBuildingTypes[typeIntPair.first]--;
		if (typeIntPair.second == 0)
		{
			tiedDownBuildingTypes.erase(typeIntPair.first);
		}
	}

	for (Unit currUnit : assignedUnits)
	{
		if (latencyFrames[currUnit] > 0)
		{
			latencyFrames[currUnit]--;
			continue;
		}

		if (isIdleBuilding(currUnit))
		{
			idleBuildings.insert(currUnit);
		}
		else
		{
			idleBuildings.erase(currUnit);
		}
	}
}

bool ZerGreenAI::ProductionManager::isIdleBuilding(BWAPI::Unit u)
{
	return u->getTrainingQueue().size() <= 0 && tiedDownBuildingTypes.count(u->getType()) == 0;
}

std::vector<BWAPI::UnitType> ZerGreenAI::ProductionManager::unassignedBuildingTypes()
{
	std::vector<UnitType> returnValue;
	for (Unit u : idleBuildings)
	{
		if (std::find(returnValue.begin(), returnValue.end(), u->getType()) == returnValue.end())
		{
			returnValue.push_back(u->getType());
		}
	}
	return returnValue;
}

bool ZerGreenAI::ProductionManager::produceType(BWAPI::UnitType whatType)
{
	for (Unit u : Unitset(idleBuildings))
	{
		if (u->canTrain(whatType, false))
		{
			idleBuildings.erase(u);
			latencyFrames[u] = Broodwar->getLatencyFrames() + 1;
			return u->train(whatType);
		}
	}
	return false;
}

void ZerGreenAI::ProductionManager::tieDown(BWAPI::UnitType whatType)
{
	tiedDownBuildingTypes[whatType] = TIED_DOWN_DURATION;
}

