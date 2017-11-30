#include "ProbeScout.h"
#include "GlobalHarvesting.h"
#include "Hashes.h"
#include "ScoutAnalysis.h"
#include "IMPScoutManager.h"

Unit probe;

void ProbeScoutManager::recycleUnit(Unit u)
{
	this->cleanMeUp = true;
}

ProbeScoutManager::ProbeScoutManager(Unit u)
{
	probe = u;
	if (!requestUnitManagement(u))
	{
		Broodwar << "Error: ProbeScoutManager given probe without consent" << std::endl;
	}
}

void ProbeScoutManager::onFrame()
{
	if (Broodwar->isVisible((TilePosition)probe->getTargetPosition()))
	{
		probe->move((Position)getIMPScoutManager()->getColdest());
	}
}

void startProbeScout()
{
	new ProbeScoutManager(getGlobalHarvester()->nearbyAvailableHarvester(Position(Broodwar->getStartLocations().front())));
}