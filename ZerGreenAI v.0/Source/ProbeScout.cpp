#include "stdafx.h"

#include "ZerGreenAI.hpp"
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
		probe->move((Position)ZerGreenAIObj::mainInstance->impScoutManager->getColdest());
	}
}

void ZerGreenAI::startProbeScout()
{
	Unit probe = ZerGreenAIObj::mainInstance->globalHarvestManager->nearbyAvailableHarvester(Position(Broodwar->getStartLocations().front()));
	if (probe == nullptr)
	{
		Broodwar << "Error: failed to find probe for scouting" << std::endl;
		return;
	}
	else
	{
		new ProbeScoutManager(probe);
	}
}