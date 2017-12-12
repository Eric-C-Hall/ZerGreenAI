#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "UnitManagement.hpp"
#include "ResourceAllocator.hpp"
#include "GlobalHarvesting.hpp"
#include "Production.hpp"
#include "Upgrade.hpp"
#include "Construction.hpp"
#include "Timer.hpp"
#include "LayoutPlanner.hpp"
#include "CombatStrategist.hpp"
#include "Namespaces.hpp"

std::unordered_map<Unit, UnitManager*> Unit2Manager;

UnitManager::~UnitManager()
{
	if (Broodwar->isInGame())
	{
		for (auto const &u : Unitset(assignedUnits)) // New unitset created because assignedUnits is modified within loop
		{
			Unit2Manager.erase(u);
			giveUnitManagement(u, ZerGreenAIObj::mainInstance->resourceAllocator);
		}
	}
}

void UnitManager::giveUnitManagement(Unit u, UnitManager * mgr)
{
	assert(assignedUnits.contains(u));
	onReassignment(u);

	assignedUnits.erase(u);
	Unit2Manager.erase(u);
	if (mgr != nullptr)
	{
		mgr->giveOrphanUnit(u);
	}

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

UnitManager* ZerGreenAI::getUnitManager(Unit u)
{
	return Unit2Manager[u];
}

#define SHOW_OWNER 0

void ZerGreenAI::UnitManager::onUnitDestroy(BWAPI::Unit u)
{
	if (assignedUnits.contains(u))
	{
		giveUnitManagement(u, nullptr);
	}
}

void ZerGreenAI::UnitManager::onUnitRenegade(BWAPI::Unit u)
{
	if (assignedUnits.contains(u))
	{
		giveUnitManagement(u, nullptr);
	}
}


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