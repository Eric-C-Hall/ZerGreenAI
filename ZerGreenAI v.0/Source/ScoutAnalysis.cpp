#include "ScoutAnalysis.h"
#include "MapAnalyser.h"
#include "Debug.h"

std::map<const BWEM::Area *, int> targets;

void scoutAnalysisOnDiscover(Unit u)
{
	const BWEM::Area * closestArea;
	closestArea = theMap.GetNearestArea(u->getTilePosition());

	if (IsEnemy(u))
	{
		new debugText(Position(areaCenter(closestArea)), std::to_string(targets[closestArea]), 96);

		if (IsResourceDepot(u))
		{
			targets[closestArea] += 5;
		}
		else if (IsBuilding(u))
		{
			targets[closestArea] += 1;
		}
	}
}

std::map<const BWEM::Area *, int> getTargets()
{
	return targets;
}

const BWEM::Area * getHighestPriorityTarget()
{
	int bestScore = INT_MIN;
	const BWEM::Area * bestArea = nullptr;

	for (auto const &t : targets)
	{
		if (t.second > bestScore)
		{
			bestScore = t.second;
			bestArea = t.first;
		}
	}

	return bestArea;

}