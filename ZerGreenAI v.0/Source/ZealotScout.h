#pragma once

#include "Namespaces.h"
#include "Management.h"

class ZealotScoutManager : public JuniorManager
{
	void recycleUnit(Unit u);
public:
	inline std::string name() { return "Zealot Scout Manager"; }
	ZealotScoutManager(Unit u);
	void onFrame();
};
