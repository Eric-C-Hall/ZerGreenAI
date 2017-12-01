#pragma once

#include "UnitManagement.hpp"
#include "Hashes.hpp"

namespace ZerGreenAI
{

	class UpgradeManager : public UnitManager
	{
		std::unordered_set<BWAPI::UpgradeType> ChosenUpgrades;
		std::unordered_set<BWAPI::TechType> ChosenTechTypes;

	public:
		inline std::string name() { return "Upgrade Manager"; }
		void onFrame();

		void ChooseTech(BWAPI::TechType tech);
		void ChooseUpgrade(BWAPI::UpgradeType upgrade);
	};

	UpgradeManager * getUpgradeManager();

}