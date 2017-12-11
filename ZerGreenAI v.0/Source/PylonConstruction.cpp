#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "Construction.hpp"
#include "PylonConstruction.hpp"

#define SUPPLY_LEEWAY ( Broodwar->self()->supplyTotal() < 25 ? 4 : 18)

// Doesn't count refinery
std::unordered_map<UnitType, int> numOfType;

// From BuildOrderOld
bool pylonNeccessary()
{
	int expectedGain = numOfType[UnitTypes::Protoss_Nexus] * 2 + numOfType[UnitTypes::Protoss_Gateway] * 4 + numOfType[UnitTypes::Protoss_Stargate] * 6 + numOfType[UnitTypes::Protoss_Robotics_Facility] * 8 + SUPPLY_LEEWAY;
	int expectedAmount = Broodwar->self()->supplyUsed() + expectedGain;
	// expectedCap is calculated this way to account for Pylons/Nexi that are unfinished
	int expectedCap = numOfType[UnitTypes::Protoss_Pylon] * UnitTypes::Protoss_Pylon.supplyProvided() + numOfType[UnitTypes::Protoss_Nexus] * UnitTypes::Protoss_Nexus.supplyProvided();
	return (expectedCap < expectedAmount);
}

void ZerGreenAI::PylonConstructionManager::onFrame()
{
	if (pylonNeccessary() && Broodwar->canMake(UnitTypes::Protoss_Pylon))
	{
		ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(UnitTypes::Protoss_Pylon);
		return;
	}
}

void ZerGreenAI::PylonConstructionManager::onUnitCreate(BWAPI::Unit u)
{
	if ((IsOwned && IsBuilding)(u))
	{
		numOfType[u->getType()]++;
	}
}

void ZerGreenAI::PylonConstructionManager::onUnitDestroy(BWAPI::Unit u)
{
	if (IsOwned(u) && u->getType() == UnitTypes::Protoss_Pylon)
	{
		numOfType[u->getType()]--;
	}
}
