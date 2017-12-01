#include "IMPScoutManager.hpp"
#include "bwemL.hpp"
#include "Debug.hpp"
#include "Namespaces.hpp"

IMPScoutManager localIMPScoutManager;

void IMPScoutManager::onStart()
{
	for (int x = 0; x < Broodwar->mapWidth(); x+=IMP_NUM_TILEPOS_SKIPPED)
	{
		for (int y = 0; y < Broodwar->mapHeight(); y+= IMP_NUM_TILEPOS_SKIPPED)
		{
			TilePosition currentPos(x, y);
			if (Broodwar->isWalkable((WalkPosition)currentPos))
			{
				heatMap[currentPos] = 0;
			}
		}
	}

	for (const TilePosition &currentPos : Broodwar->getStartLocations())
	{
		heatMap[currentPos] = 1000;
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
			heatMap[(TilePosition)b.Center()] += 5;
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

IMPScoutManager * ZerGreenAI::getIMPScoutManager()
{
	return &localIMPScoutManager;
}
