#pragma once

#include "GeneralManagement.hpp"
#include "Hashes.hpp"

namespace ZerGreenAI
{

#define IMP_NUM_TILEPOS_SKIPPED 5
#define IMP_FRAMES_PER_UPDATE 20
#define IMP_INDICATOR_SIZE_MODIFIER 50
#define IMP_INITIAL_STARTLOCATION_SIZE 2000
#define IMP_EXPANSION_INCREMENT 5

	class IMPScoutManager : protected Manager
	{
		std::unordered_map<BWAPI::TilePosition, int> heatMap;

		void onStart();
		void onFrame();

	public:
		inline virtual std::string name() { return "IMP Scouting Manager"; }
		BWAPI::TilePosition getColdest();
		BWAPI::TilePosition getColdestWeightingDistance(BWAPI::TilePosition startP, int weight);
	};
}