#pragma once

#include "Namespaces.h"
#include "UnitManagement.h"

class ConstructionManager : public UnitManager
{
	bool acceptRequest(Unit u); // Preferably should blacklist rather than whitelist
	void recycleUnit(Unit u);
public:
	inline std::string name() { return "Construction Manager"; }
	void onFrame();

	bool constructBuilding(UnitType type);
};

ConstructionManager * getConstructionManager();