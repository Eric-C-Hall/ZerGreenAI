#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "ProbeScout.hpp"
#include "GlobalHarvesting.hpp"
#include "Hashes.hpp"
#include "ScoutAnalysis.hpp"
#include "IMPScoutManager.hpp"
#include "Namespaces.hpp"
#include "Debug.hpp"

Unit probe;

void ZerGreenAI::ProbeScoutManager::onAssignment(BWAPI::Unit u)
{
	probe = u;
}

void ProbeScoutManager::onReassignment(Unit u)
{
	delete this;
}

void ProbeScoutManager::onFrame()
{
	UnitManager::onFrame();
	if (Broodwar->isVisible((TilePosition)probe->getTargetPosition()))
	{
		probe->move((Position)ZerGreenAIObj::mainInstance->impScoutManager->getColdestWeightingDistance((TilePosition)probe->getPosition()));
	}

	chatterTime++;
	if (chatterTime > chatterDuration)
	{
		chatterTime = 0;
		chatterDuration = (rand() % (CHATTER_MAX_DURATION - CHATTER_MIN_DURATION)) + CHATTER_MIN_DURATION;
		currentChatter = chatter[rand() % chatter.size()];
		debugUnitText(probe, currentChatter, chatterDuration);
	}
}

void ZerGreenAI::startProbeScout()
{
	ProbeScoutManager * newManager = new ProbeScoutManager;
	Unit probe = ZerGreenAIObj::mainInstance->globalHarvestManager->giveNearbyAvailableHarvester(Position(Broodwar->getStartLocations().front()),newManager);
	if (probe == nullptr)
	{
		Broodwar << "Error: failed to find probe for scouting" << std::endl;
		delete newManager;
	}
}