#pragma once

#include "Namespaces.h"
#include "Management.h"

class ConstructionManager : public JuniorManager
{
	bool acceptRequest(Unit u); // Preferably should blacklist rather than whitelist
	void recycleUnit(Unit u);
public:
	inline std::string name() { return "Construction Manager"; }
	void onFrame();

	bool constructBuilding(UnitType type);
};

void initializeConstructionManager();
ConstructionManager * getConstructionManager();