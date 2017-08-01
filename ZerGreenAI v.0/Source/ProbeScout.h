#pragma once
#include "Namespaces.h"
#include "Management.h"

class ProbeScoutManager : public JuniorManager
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