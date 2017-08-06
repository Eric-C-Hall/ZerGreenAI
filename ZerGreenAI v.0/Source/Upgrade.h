#pragma once

#include "Namespaces.h"
#include "UnitManagement.h"
#include "Hashes.h"

class UpgradeManager : public UnitManager
{
	std::unordered_set<UpgradeType> ChosenUpgrades;
	std::unordered_set<TechType> ChosenTechTypes;

public:
	inline std::string name() { return "Upgrade Manager"; }
	void onFrame();

	void ChooseTech(TechType tech);
	void ChooseUpgrade(UpgradeType upgrade);
};

UpgradeManager * getUpgradeManager();