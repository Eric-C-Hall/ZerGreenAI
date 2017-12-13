#include "stdafx.h"

#include "IMPScoutManager.hpp"
#include "bwemL.hpp"
#include "Debug.hpp"
#include "Namespaces.hpp"

void IMPScoutManager::onStart()
{
	for (int x = 0; x < Broodwar->mapWidth(); x+=IMP_NUM_TILEPOS_SKIPPED)
	{
		for (int y = 0; y < Broodwar->mapHeight(); y+= IMP_NUM_TILEPOS_SKIPPED)
		{
			TilePosition currentPos(x, y);
			if (Broodwar->hasPath((Position)Broodwar->self()->getStartLocation(),(Position)currentPos))
			{
				heatMap[currentPos] = 0;
			}
		}
	}

	for (const TilePosition &currentPos : Broodwar->getStartLocations())
	{
		heatMap[currentPos] = IMP_INITIAL_STARTLOCATION_SIZE;
	}
}

void IMPScoutManager::onFrame()
{
	if (Broodwar->getFrameCount() % IMP_FRAMES_PER_UPDATE != 0)
		return;

	for (const auto &tileHeat : heatMap)
	{
		new debugCircle((Position)tileHeat.first, tileHeat.second / IMP_INDICATOR_SIZE_MODIFIER, Colors::Yellow, IMP_FRAMES_PER_UPDATE);

		if (Broodwar->isVisible(tileHeat.first))
			heatMap[tileHeat.first] = 0;
		else
			heatMap[tileHeat.first]++;
	}

	for (const BWEM::Area &a : theMap.Areas())
	{
		for (const BWEM::Base &b : a.Bases())
		{
			if (Broodwar->hasPath((Position)Broodwar->self()->getStartLocation(), b.Center()))
				heatMap[(TilePosition)b.Center()] += IMP_EXPANSION_INCREMENT;
		}
	}


}

TilePosition IMPScoutManager::getColdest()
{
	TilePosition bestPos;
	int bestValue = INT_MIN;
	for (const auto &tileHeat : heatMap)
	{
		if (tileHeat.second > bestValue)
		{
			bestPos = tileHeat.first;
			bestValue = tileHeat.second;
		}
	}
	return bestPos;
}

BWAPI::TilePosition ZerGreenAI::IMPScoutManager::getColdestWeightingDistance(BWAPI::TilePosition startP, double weight)
{
	TilePosition bestPos;
	int bestValue = INT_MIN;
	for (const auto &tileHeat : heatMap)
	{
		int currentValue = tileHeat.second - (int)(weight * startP.getApproxDistance(tileHeat.first));
		if ( currentValue > bestValue)
		{
			bestPos = tileHeat.first;
			bestValue = currentValue;
		}
	}
	return bestPos;
}
