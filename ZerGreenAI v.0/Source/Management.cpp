#include "Management.h"
#include "ResourceAllocator.h"
#include "GlobalHarvesting.h"
#include "Production.h"
#include "Upgrade.h"
#include "Construction.h"
#include "Timer.h"
#include "LayoutPlanner.h"
#include "BuildOrder.h"
#include "CombatStrategist.h"

std::unordered_map<Unit, JuniorManager*> Unit2Manager;
std::unordered_set<JuniorManager*> Managers;
JuniorManager* LocalResourceAllocator;



JuniorManager::~JuniorManager()
{
	for (auto const &u : assignedUnits)
	{
		Unit2Manager.erase(u);
		giveUnitManagement(u, LocalResourceAllocator);
	}
}

bool JuniorManager::requestUnitManagement(Unit u)
{
	JuniorManager * uMgr = getUnitManager(u);
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

void JuniorManager::giveUnitManagement(Unit u, JuniorManager * mgr)
{
	acceptRequest(u); // Called in order to have side effects, result ignored.

	assignedUnits.erase(u);
	Unit2Manager[u] = nullptr;
	mgr->giveOrphanUnit(u);
}

bool JuniorManager::giveOrphanUnit(Unit u)
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

void JuniorManager::recycleUnitJunior(Unit u)
{
	recycleUnit(u);
	Unit2Manager.erase(u);
	assignedUnits.erase(u);
}

void addManager(JuniorManager * mgr)
{
	Managers.insert(mgr);
}

void deleteManager(JuniorManager * mgr)
{	
	Managers.erase(mgr);
	delete mgr;
}

std::unordered_set<JuniorManager *> managersToBeDeleted;

void deleteManagerWhenPossible(JuniorManager * mgr)
{
	managersToBeDeleted.insert(mgr);
}

JuniorManager* getUnitManager(Unit u)
{
	return Unit2Manager[u];
}

#define SHOW_OWNER 0

void onFrameSeniorManager()
{
	for (auto const &m : managersToBeDeleted)
	{
		deleteManager(m);
	}
	managersToBeDeleted.clear();

	for (auto const &m : Managers)
	{
		startTimer(m->name());
		m->onFrame();
		endTimer(m->name());
	}

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

void recycleUnitSenior(Unit u)
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

JuniorManager* getResourceAllocator()
{
	return LocalResourceAllocator;
}



void initializeManagement()
{
	LocalResourceAllocator = new ResourceAllocator;
	addManager(LocalResourceAllocator);
	initializeGlobalHarvester();
	initializeProductionManager();
	initializeUpgradeManager();
	initializeConstructionManager();
	initializeLayoutPlanner();
	initializeCombatStrategist();
}