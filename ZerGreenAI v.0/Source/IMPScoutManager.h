#pragma once
#include "Namespaces.h"
#include "GeneralManagement.h"
#include "Hashes.h"

#define IMP_NUM_TILEPOS_SKIPPED 5
#define IMP_FRAMES_PER_UPDATE 20
#define IMP_INDICATOR_SIZE_MODIFIER 50

class IMPScoutManager : protected Manager
{
	std::unordered_map<TilePosition, int> heatMap;

	void onStart();
	void onFrame();

public:
	inline std::string name() { return "IMP Scouting Manager"; }
	TilePosition getColdest();
	TilePosition getColdestWeightingDistance(TilePosition startP, int weight);
};

IMPScoutManager * getIMPScoutManager();