#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "Construction.hpp"
#include "PylonConstruction.hpp"
#include "UnitsOfTypeCounter.hpp"

#define SUPPLY_LEEWAY ( Broodwar->self()->supplyTotal() < 25 ? 4 : 18)

// From BuildOrderOld
bool pylonNeccessary()
{
	int expectedGain = ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Nexus) * 2 + ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Gateway) * 4 + ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Stargate) * 6 + ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Robotics_Facility) * 8 + SUPPLY_LEEWAY;
	int expectedAmount = Broodwar->self()->supplyUsed() + expectedGain;
	// expectedCap is calculated this way to account for Pylons/Nexi that are unfinished
	int expectedCap = ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Pylon) * UnitTypes::Protoss_Pylon.supplyProvided() + ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Nexus) * UnitTypes::Protoss_Nexus.supplyProvided();
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
