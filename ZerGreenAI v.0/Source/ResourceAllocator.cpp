#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "ResourceAllocator.hpp"
#include "LocalHarvesting.hpp"
#include "GlobalHarvesting.hpp"
#include "Production.hpp"
#include "Upgrade.hpp"
#include "LayoutPlanner.hpp"
#include "CombatStrategist.hpp"
#include "Namespaces.hpp"

UnitManager * whoToGive(Unit u)
{
	if (!IsCompleted(u))
	{
		return nullptr;
	}

	if ((IsWorker || IsResourceDepot)(u))
	{
		return ZerGreenAIObj::mainInstance->globalHarvestManager;
	}
	else if (IsBuilding(u))
	{
		bool canUpgrade = u->getType().upgradesWhat().size() > 0 || u->getType().researchesWhat().size() > 0;
		
		if (CanProduce(u))
		{
			assert(!canUpgrade);
			return ZerGreenAIObj::mainInstance->productionManager;
		}
		else if (canUpgrade)
		{
			return ZerGreenAIObj::mainInstance->upgradeManager;
		}
		return nullptr;

	}
	else
	{
		return ZerGreenAIObj::mainInstance->combatStrategist;
	}
	return nullptr;
}

void ResourceAllocator::onFrame()
{
	std::map<Unit, UnitManager *> intermediateStorage;

	for (auto const &u : assignedUnits)
	{
		intermediateStorage[u] = whoToGive(u);
	}

	for (auto const &m : intermediateStorage)
	{
		if (m.second != nullptr)
		{
			giveUnitManagement(m.first, m.second);
		}
	}

}