#pragma once

#include "Namespaces.h"
#include "UnitManagement.h"

class ZealotScoutManager : public UnitManager
{
	void recycleUnit(Unit u);
public:
	inline std::string name() { return "Zealot Scout Manager"; }
	ZealotScoutManager(Unit u);
	void onFrame();
};
