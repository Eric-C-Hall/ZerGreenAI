#pragma once

#include "Namespaces.h"

class JuniorManager
{
	virtual void onAssignment(Unit u) {};
	virtual bool acceptRequest(Unit u) { return true; }; // Preferably should blacklist rather than whitelist
	virtual void recycleUnit(Unit u) {};

protected:

	Unitset assignedUnits;
	void giveUnitManagement(Unit u, JuniorManager * mgr);
	bool requestUnitManagement(Unit u);

public:
	inline virtual std::string name() { return "Name not Found"; }
	virtual void onFrame() {};
	virtual ~JuniorManager();

	bool giveOrphanUnit(Unit u);
	void recycleUnitJunior(Unit u);


};

void addManager(JuniorManager * mgr);
void deleteManager(JuniorManager * mgr);
void deleteManagerWhenPossible(JuniorManager * mgr);
JuniorManager* getUnitManager(Unit u);
void onFrameSeniorManager();
void recycleUnitSenior(Unit u);

void initializeManagement();
JuniorManager* getResourceAllocator();
