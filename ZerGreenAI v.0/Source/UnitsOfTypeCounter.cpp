#include "stdafx.h"

#include "UnitsOfTypeCounter.hpp"

void ZerGreenAI::UnitsOfTypeCounter::onUnitCreate(BWAPI::Unit u)
{
	if ((IsOwned && IsBuilding)(u))
	{
		numOfType[u->getType()]++;
	}
}

void ZerGreenAI::UnitsOfTypeCounter::onUnitDestroy(BWAPI::Unit u)
{
	if (IsOwned(u) && u->getType() == UnitTypes::Protoss_Pylon)
	{
		numOfType[u->getType()]--;
	}
}

void ZerGreenAI::UnitsOfTypeCounter::onUnitMorph(BWAPI::Unit u)
{
	if (IsOwned(u) && IsRefinery(u))
	{
		numOfType[u->getType()]++;
	}
}