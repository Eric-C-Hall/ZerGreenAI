#pragma once

#include "Management.h"

// Aim to be using or saving resources for goals.

// Need to balance army, tech, eco.

class ProductionManager : public JuniorManager
{
	bool executeContract(UnitType utype);
public:
	void onFrame();

	Unitset getProductionBuildings();
};

void initializeProductionManager();
ProductionManager* getProductionManager();
