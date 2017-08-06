#pragma once
#include "Namespaces.h"
#include "UnitManagement.h"

class ProbeScoutManager : public UnitManager
{
	static TilePosition enemyBase;
	void recycleUnit(Unit u);
public:
	inline std::string name() { return "Probe Scout Manager"; }
	ProbeScoutManager(Unit u);
	void onFrame();
	static void foundBase(TilePosition t);
};

void startProbeScout();