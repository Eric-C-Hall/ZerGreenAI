#include "stdafx.h"

#include <fstream>

#include "BuildOrder.hpp"

void ZerGreenAI::BuildOrder::mutate(double chanceToRandomize, double chanceToCreate, double chanceToDelete)
{
	for (unsigned i = 0; i < currentBuild.size(); i++)
	{
		if (trueWithProb(chanceToRandomize))
		{
			currentBuild[i] = randomProtossUnitType();
		}

		if (trueWithProb(chanceToCreate))
		{
			currentBuild.insert(currentBuild.begin() + i + 1, randomProtossUnitType());
		}

		if (trueWithProb(chanceToDelete) && currentBuild.size() > 1)
		{
			currentBuild.erase(currentBuild.begin() + i);
		}
	}
}

BWAPI::UnitType ZerGreenAI::BuildOrder::getNext()
{
	if (currentPosition == currentBuild.size())
	{
		currentPosition = 0;
	}
	currentPosition++;
	return currentBuild[currentPosition-1];
}

ZerGreenAI::BuildOrder::BuildOrder(std::string filePath)
{
	this->filePath = filePath;
	std::ifstream buildOrderFile(filePath);
	while (buildOrderFile)
	{
		int currentUnit;
		buildOrderFile >> currentUnit;
		currentBuild.push_back(currentUnit);
	}
	buildOrderFile.close();
}

void ZerGreenAI::BuildOrder::write()
{
	std::ofstream buildOrderFile(filePath, std::ofstream::out | std::ofstream::trunc);
	for (unsigned i = 0; i < currentBuild.size(); i++)
	{
		buildOrderFile << (int)currentBuild[i] << ' ';
	}
	buildOrderFile << std::endl;
	buildOrderFile.close();
}

void ZerGreenAI::BuildOrder::debugDraw()
{
	if (debug)
	{
		int y = 0;
		for (unsigned i = 0; i < currentBuild.size();i++)
		{
			if (i == currentPosition)
			{
				Broodwar->drawTextScreen(Position(5, y), "%c%s", BWAPI::Text::Green, currentBuild[i].c_str());
			}
			else
			{
				Broodwar->drawTextScreen(Position(5, y), currentBuild[i].c_str());
			}
			y += 10;
		}
	}
}

