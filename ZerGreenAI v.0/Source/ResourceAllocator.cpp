#include "ResourceAllocator.h"
#include "LocalHarvesting.h"
#include "GlobalHarvesting.h"
#include "Production.h"
#include "Upgrade.h"
#include "LayoutPlanner.h"
#include "CombatStrategist.h"

UnitManager * whoToGive(Unit u)
{
	if ((IsWorker || IsResourceDepot)(u))
	{
		if (!IsCompleted(u))
		{
			return nullptr;
		}
		return getGlobalHarvester();
	}
	else if (IsBuilding(u))
	{
		bool canUpgrade = u->getType().upgradesWhat().size() > 0 || u->getType().researchesWhat().size() > 0;
		
		if (CanProduce(u))
		{
			assert(!canUpgrade);
			return getProductionManager();
		}
		else if (canUpgrade)
		{
			return getUpgradeManager();
		}
		else if (u->getType() == UnitTypes::Protoss_Pylon)
		{
			return getLayoutPlanner();
		}
		return nullptr;

	}
	else
	{
		return getCombatStrategist();
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