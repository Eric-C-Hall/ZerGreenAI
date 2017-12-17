#include "stdafx.h"

#include "UnitsOfTypeCounter.hpp"

void ZerGreenAI::UnitsOfTypeCounter::onUnitCreate(BWAPI::Unit u)
{
	if ((IsOwned && IsBuilding)(u))
	{
		numUnitsOfTypeMap[u->getType()]++;
	}
}

void ZerGreenAI::UnitsOfTypeCounter::onUnitDestroy(BWAPI::Unit u)
{
	if ((IsOwned && IsBuilding)(u))
	{
		numUnitsOfTypeMap[u->getType()]--;
	}
}

void ZerGreenAI::UnitsOfTypeCounter::onUnitMorph(BWAPI::Unit u)
{
	if ((IsOwned && IsRefinery)(u))
	{
		numUnitsOfTypeMap[u->getType()]++;
	}
	else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
	{
		numUnitsOfTypeMap[Broodwar->self()->getRace().getRefinery()]--;
	}
}