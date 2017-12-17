#pragma once

#include "ProductionBrain.hpp"

template<int type>
bool ZerGreenAI::ProductionBrain<type>::actionIsValid(BWAPI::UnitType action)
{
	return Broodwar->canMake(action);
}

template<int type>
std::vector<BWAPI::UnitType> ZerGreenAI::ProductionBrain<type>::getPossibleActions()
{
	std::vector<BWAPI::UnitType> returnValue;
	for (BWAPI::UnitType buildableType : ((BWAPI::UnitType)type).buildsWhat())
	{
		if (actionIsValid(buildableType))
			returnValue.push_back(buildableType);
	}
	return returnValue;
}

template<int type>
std::vector<float> ZerGreenAI::ProductionBrain<type>::getInput(BWAPI::UnitType whatAction)
{
	std::vector<float> actionVector;
	auto buildsWhatSet = ((BWAPI::UnitType)type).buildsWhat();
	// std::set used to ensure an ordering, though may not be neccessary
	for (BWAPI::UnitType builtType : std::set<BWAPI::UnitType>(buildsWhatSet.begin(), buildsWhatSet.end()))
	{
		actionVector.push_back(builtType == whatAction);
	}
	std::vector<float> perceptVector = { (float)Broodwar->self()->minerals(), (float)Broodwar->self()->gas(), (float)Broodwar->getLatencyFrames() };
	
	actionVector.insert(actionVector.end(), perceptVector.begin(), perceptVector.end());
	return actionVector;
}

template<int type>
void ZerGreenAI::ProductionBrain<type>::onStart()
{
}

template<int type>
ZerGreenAI::ProductionBrain<type>::ProductionBrain() : Brain(0.01f, 0.1f)
{
}
