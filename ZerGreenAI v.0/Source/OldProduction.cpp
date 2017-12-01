#include "OldProduction.h"
#include "OldBuildOrder.h"
#include "Namespaces.h"

ProductionManager* LocalProductionManager;

BuildPlans plans;

# define FRAMES_PER_UPDATE_CONTRACTS 29

bool ProductionManager::executeContract(UnitType utype)
{
	if (utype.isBuilding())
	{
		// TODO: Add Construction to Production
		return true;
	}

	// Broodwar->canMake();

	for (auto const &p : assignedUnits)
	{
		if (p->canTrain(utype))
		{
			p->train(utype);
			return true;
		}
	}
	return false;
}

void ProductionManager::onFrame()
{
	if (Broodwar->getFrameCount() % FRAMES_PER_UPDATE_CONTRACTS == 0)
	{
		plans.planContracts();
	}
	
	while (!plans.contracts.empty() && executeContract(plans.contracts.front()))
	{
		plans.contracts.pop();
	}

}

Unitset ProductionManager::getProductionBuildings()
{
	return assignedUnits;
}


ProductionManager * getProductionManager()
{
	return LocalProductionManager;
}

void initializeProductionManager()
{
	LocalProductionManager = new ProductionManager;
	addManager(LocalProductionManager);
}