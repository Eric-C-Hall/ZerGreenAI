#include "stdafx.h"

#include "ConstructionUtility.hpp"
#include "ZerGreenAI.hpp"
#include "GlobalHarvesting.hpp"
#include "UnitsOfTypeCounter.hpp"
#include "Construction.hpp"

bool ZerGreenAI::isBuildingReasonableBuildChoice(UnitType action)
{
	if (action == Broodwar->self()->getRace().getRefinery())
	{
		return ZerGreenAIObj::mainInstance->globalHarvestManager->numClaimedBases() > ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(UnitTypes::Protoss_Assimilator) + ZerGreenAIObj::mainInstance->constructionManager->isMakingBuilding(Broodwar->self()->getRace().getRefinery());
	}

	for (auto reqType : action.requiredUnits())
	{
		if (reqType.first == UnitTypes::Protoss_Pylon || reqType.first == UnitTypes::Protoss_Probe)
			continue;

		if (ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(reqType.first) < reqType.second)
		{
			return false;
		}
	}

	return true;
}