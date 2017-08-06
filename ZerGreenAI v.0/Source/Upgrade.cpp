#include "Upgrade.h"

UpgradeManager LocalUpgradeManager;

void UpgradeManager::onFrame()
{
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	for (auto const &u : assignedUnits)
	{
		if (!u->isCompleted())
		{
			continue;
		}

		for (auto const &t : u->getType().upgradesWhat())
		{
			if (ChosenUpgrades.count(t) > 0)
			{
				u->upgrade(t);
			}
		}

		for (auto const &t : u->getType().researchesWhat())
		{
			if (ChosenTechTypes.count(t) > 0)
			{
				u->research(t);
			}
		}
	}
}


void UpgradeManager::ChooseTech(TechType tech)
{
	ChosenTechTypes.insert(tech);
}

void UpgradeManager::ChooseUpgrade(UpgradeType upgrade)
{
	ChosenUpgrades.insert(upgrade);
}

UpgradeManager * getUpgradeManager()
{
	return &LocalUpgradeManager;
}