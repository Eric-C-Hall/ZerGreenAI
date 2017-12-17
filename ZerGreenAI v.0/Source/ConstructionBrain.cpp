#include "stdafx.h"

#include <sstream>

#include "ZerGreenAI.hpp"
#include "Construction.hpp"
#include "ConstructionBrain.hpp"
#include "UnitsOfTypeCounter.hpp"
#include "Debug.hpp"
#include "GlobalHarvesting.hpp"
#include "ConstructionUtility.hpp"

#if USING_N00BY_EDGE
#include "ModularNN.h"
#endif


const std::vector<BWAPI::UnitType> ZerGreenAI::ConstructionBrain::protossBuildingTypesExcludingPylon = { UnitTypes::Protoss_Arbiter_Tribunal, UnitTypes::Protoss_Assimilator, UnitTypes::Protoss_Citadel_of_Adun, UnitTypes::Protoss_Cybernetics_Core, UnitTypes::Protoss_Fleet_Beacon, UnitTypes::Protoss_Forge, UnitTypes::Protoss_Gateway, UnitTypes::Protoss_Nexus, UnitTypes::Protoss_Observatory, UnitTypes::Protoss_Photon_Cannon, UnitTypes::Protoss_Robotics_Facility, UnitTypes::Protoss_Shield_Battery, UnitTypes::Protoss_Stargate, UnitTypes::Protoss_Templar_Archives };

std::vector<UnitType> ZerGreenAI::ConstructionBrain::getPossibleActions()
{
	std::vector<UnitType> possibleActions;
	for (UnitType action : protossBuildingTypesExcludingPylon)
	{
		if (actionIsValid(action))
		{
			possibleActions.push_back(action);
		}
	}
	return possibleActions;
}

void ZerGreenAI::ConstructionBrain::onStart()
{
	
}

std::vector<float> ConstructionBrain::getInput(UnitType whatAction)
{
	std::vector<float> returnVector;
	for (UnitType t : protossBuildingTypesExcludingPylon)
	{
		for (int i = 0; i < LIMIT_TO_DISTINCT_NUMBERS_OF_BUILDINGS; i++)
		{
			if (ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(t) == i)
			{
				returnVector.push_back(1.0);
			}
			else
			{
				returnVector.push_back(0.0);
			}
		}
		if (ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(t) >= LIMIT_TO_DISTINCT_NUMBERS_OF_BUILDINGS)
		{
			returnVector.push_back(1.0);
		}
		else
		{
			returnVector.push_back(0.0);
		}
	}
	for (UnitType t : protossBuildingTypesExcludingPylon)
	{
		if (t == whatAction)
		{
			returnVector.push_back(1.0);
		}
		else
		{
			returnVector.push_back(0);
		}
	}
	return returnVector;
}

bool ZerGreenAI::ConstructionBrain::actionIsValid(UnitType action)
{
	return isBuildingReasonableBuildChoice(action);
}

void ZerGreenAI::ConstructionBrain::drawDebug()
{
	if (displayPossibleUnits)
	{
		int currentY = 0;
		currentY += 10;
		for (auto type : protossBuildingTypesExcludingPylon)
		{
			if (actionIsValid(type))
			{
				Broodwar->drawTextScreen(Position(5, currentY), type.c_str());
				currentY += 10;
			}
		}
		if (currentY == 0)
		{
			Broodwar->drawTextScreen(Position(5, currentY), "No Units Possible");
			currentY += 10;
		}
	}
}

ZerGreenAI::ConstructionBrain::ConstructionBrain() : Brain(0.01f, 0.1f) {}