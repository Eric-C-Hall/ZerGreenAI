#include "UnitManagement.hpp"
#include "ResourceAllocator.hpp"
#include "GlobalHarvesting.hpp"
#include "Production.hpp"
#include "Upgrade.hpp"
#include "Construction.hpp"
#include "Timer.hpp"
#include "LayoutPlanner.hpp"
#include "BuildOrder.hpp"
#include "CombatStrategist.hpp"
#include "Namespaces.hpp"

std::unordered_map<Unit, UnitManager*> Unit2Manager;
ResourceAllocator LocalResourceAllocator;

UnitManager::~UnitManager()
{
	if (Broodwar->isInGame())
	{
		for (auto const &u : assignedUnits)
		{
			Unit2Manager.erase(u);
			giveUnitManagement(u, &LocalResourceAllocator);
		}
	}
}

bool UnitManager::requestUnitManagement(Unit u)
{
	UnitManager * uMgr = getUnitManager(u);
	if (uMgr == nullptr)
	{
		giveOrphanUnit(u);
		return true;
	}
	if (uMgr->acceptRequest(u))
	{
		uMgr->assignedUnits.erase(u);
		Unit2Manager[u] = nullptr;
		giveOrphanUnit(u);
		return true;
	}
	else
	{
		return false;
	}
}

void UnitManager::giveUnitManagement(Unit u, UnitManager * mgr)
{
	acceptRequest(u); // Called in order to have side effects, result ignored.

	assignedUnits.erase(u);
	Unit2Manager[u] = nullptr;
	mgr->giveOrphanUnit(u);
}

bool UnitManager::giveOrphanUnit(Unit u)
{
	if (Unit2Manager[u] == nullptr)
	{
		assignedUnits.insert(u);
		Unit2Manager[u] = this;
		onAssignment(u);
		return true;
	}
	return false;
}

void UnitManager::recycleUnitJunior(Unit u)
{
	Unit2Manager.erase(u);
	assignedUnits.erase(u);
	recycleUnit(u);
}

UnitManager* ZerGreenAI::getUnitManager(Unit u)
{
	return Unit2Manager[u];
}

#define SHOW_OWNER 0

void UnitManager::onFrame()
{

#if SHOW_OWNER && _DEBUG
	for (auto const &u : Broodwar->getAllUnits())
	{
		if (u->getPlayer() == Broodwar->self() && u->isCompleted())
		{
			if (Unit2Manager[u] != nullptr)
			{
				Broodwar->drawTextMap(u->getPosition(),Unit2Manager[u]->name().c_str());
			}
			else
			{
				Broodwar->drawTextMap(u->getPosition(), "No Owner");
			}
		}
	}
#endif
}

void ZerGreenAI::recycleUnitSenior(Unit u)
{
	if (Unit2Manager[u] != nullptr)
	{
		Unit2Manager[u]->recycleUnitJunior(u);
	}
	else
	{
		Unit2Manager.erase(u);
	}

	buildOrderOnRecycle(u);
}

UnitManager* ZerGreenAI::getResourceAllocator()
{
	return &LocalResourceAllocator;
}