#pragma once

#include "Namespaces.h"
#include "GeneralManagement.h"

class UnitManager : protected Manager
{
	virtual void onAssignment(Unit u) {};
	virtual bool acceptRequest(Unit u) { return true; }; // Preferably should blacklist rather than whitelist
	virtual void recycleUnit(Unit u) {};
	void onFrame();

protected:

	Unitset assignedUnits;
	void giveUnitManagement(Unit u, UnitManager * mgr);
	bool requestUnitManagement(Unit u);

public:
	virtual ~UnitManager();

	bool giveOrphanUnit(Unit u);
	void recycleUnitJunior(Unit u);
};

UnitManager* getUnitManager(Unit u);
void recycleUnitSenior(Unit u);

void initializeManagement();
UnitManager* getResourceAllocator();
