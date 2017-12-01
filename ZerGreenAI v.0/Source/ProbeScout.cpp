#include "ProbeScout.hpp"
#include "GlobalHarvesting.hpp"
#include "Hashes.hpp"
#include "ScoutAnalysis.hpp"
#include "IMPScoutManager.hpp"
#include "Namespaces.hpp"

Unit probe;

void ProbeScoutManager::recycleUnit(Unit u)
{
	delete this;
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

void ZerGreenAI::startProbeScout()
{
	new ProbeScoutManager(getGlobalHarvester()->nearbyAvailableHarvester(Position(Broodwar->getStartLocations().front())));
}