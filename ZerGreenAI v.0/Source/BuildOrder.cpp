#include "stdafx.h"

#include <sstream>

#include "ZerGreenAI.hpp"
#include "Construction.hpp"
#include "BuildOrder.hpp"
#include "ModularNN.h"

const float ZerGreenAI::BuildOrderManager::LEARNING_SPEED = 0.01f;
const float ZerGreenAI::BuildOrderManager::CHANCE_TO_RANDOMIZE_ACTION = 0.1f;

std::vector<BWAPI::UnitType> ZerGreenAI::protossBuildingTypesExcludingPylon = { UnitTypes::Protoss_Arbiter_Tribunal, UnitTypes::Protoss_Assimilator, UnitTypes::Protoss_Citadel_of_Adun, UnitTypes::Protoss_Cybernetics_Core, UnitTypes::Protoss_Fleet_Beacon, UnitTypes::Protoss_Forge, UnitTypes::Protoss_Gateway, UnitTypes::Protoss_Nexus, UnitTypes::Protoss_Observatory, UnitTypes::Protoss_Photon_Cannon, UnitTypes::Protoss_Robotics_Facility, UnitTypes::Protoss_Shield_Battery, UnitTypes::Protoss_Stargate, UnitTypes::Protoss_Templar_Archives };

std::vector<std::string> ZerGreenAI::BuildOrderManager::getFileLabelledInput(std::string fileName)
{
	std::ifstream currentFile(fileName);
	if (!currentFile.is_open())
	{
		throw "Error: unable to open " + fileName + " in getFileLabelledInput";
	}
	std::vector<std::string> returnVector;

	std::string currentLine;
	std::getline(currentFile, currentLine);
	while (currentFile)
	{
		returnVector.push_back(currentLine);
		std::getline(currentFile, currentLine);
	}

	if (returnVector.empty())
	{
		throw "Error: unable to parse" + fileName + "in getFileLabelledInput";
	}
	std::string didWinString = returnVector.back();
	returnVector.pop_back();
	for (auto i = returnVector.begin(); i != returnVector.end(); i++)
	{
		i->append(' ' + didWinString);
	}
	return returnVector;
}

inline std::string ZerGreenAI::BuildOrderManager::getSaveFileName(int i)
{
	return "bwapi-data/read/instance" + std::to_string(Broodwar->getInstanceNumber()) + "/gameinfo/buildOrder/game" + std::to_string(i) + ".bo";
}

inline std::string ZerGreenAI::BuildOrderManager::getGamenumberFileName()
{
	return "bwapi-data/read/instance" + std::to_string(Broodwar->getInstanceNumber()) + "/gameinfo/buildOrder/gamenumber.txt";
}

inline std::string getBuildOrderNNFileName()
{
	return "bwapi-data/read/instance" + std::to_string(Broodwar->getInstanceNumber()) + "/neuralNetworks/buildOrderManager.txt";
}

void ZerGreenAI::BuildOrderManager::onStart()
{
	std::ifstream gameNumberFileInput(getGamenumberFileName(), ifstream::in);
	assert(gameNumberFileInput.is_open());
	gameNumberFileInput >> gameNumber;
	gameNumberFileInput.close();

	gameNumber++;
	if (gameNumber >= NUM_REMEMBER_GAMES)
	{
		gameNumber = 0;
	}

	std::ofstream gameNumberFileOutput(getGamenumberFileName(), ifstream::out | ifstream::trunc);
	assert(gameNumberFileOutput.is_open());
	gameNumberFileOutput << gameNumber;
	gameNumberFileOutput.close();

	for (int i = 0; i < gameNumber; i++)
	{
		try
		{
			getFileLabelledInput(getSaveFileName(i));
		}
		catch (const std::string &) //e)
		{
			// Just ignore the exception. Perhaps this is a bad idea.
			//Broodwar << e << std::endl;
		}

	}

	gameFile.open(getSaveFileName(gameNumber));
}

// TODO: Do this in a cleaner, intended way. See Below comment
// This seems to turn an ifstream into a istream or something somehow
// Note: very similar to getNeuralNetworkOutputStream
auto getNeuralNetworkInputStream()
{
	return [&]() {
#ifndef SSCAIT
		std::ifstream model(getBuildOrderNNFileName());
		assert(model.is_open());
		return model;

#else
		return std::ifstream("bwapi-data/AI/model.nn");
#endif
	}();
}

// TODO: Do this in a cleaner, intended way. See Below comment
// This seems to turn an ofstream into a ostream or something somehow
// Note: very similar to getNeuralNetworkInputStream
auto getNeuralNetworkOutputStream()
{
	return [&]() {
#ifndef SSCAIT
		std::ofstream model(getBuildOrderNNFileName());
		assert(model.is_open());
		return model;

#else
		return std::ifstream("bwapi-data/AI/model.nn");
#endif
	}();
}


void ZerGreenAI::BuildOrderManager::onEnd(bool didWin)
{
	gameFile << didWin << std::endl;

	for (const std::string &experienceString : getFileLabelledInput(getSaveFileName(gameNumber)))
	{
		std::vector<float> experienceVector;
		stringstream sstream(experienceString);

		float currentFloat;
		while (sstream >> currentFloat)
		{
			experienceVector.push_back(currentFloat);
			if (sstream.peek() == ',')
				sstream.ignore();
		}

		float value = experienceVector.back();
		experienceVector.pop_back();
		neuralNetwork.updateNeuralNetwork(experienceVector, { value }, LEARNING_SPEED);
		neuralNetwork.write(getNeuralNetworkOutputStream());
	}
}

std::vector<float> BuildOrderManager::getInput(UnitType whatAction)
{
	std::vector<float> returnVector;
	for (UnitType t : protossBuildingTypesExcludingPylon)
	{
		for (int i = 0; i < LIMIT_TO_DISTINCT_NUMBERS_OF_BUILDINGS; i++)
		{
			if (numUnitsOfType[t] == i)
			{
				returnVector.push_back(1.0);
			}
			else
			{
				returnVector.push_back(0.0);
			}
		}
		if (numUnitsOfType[t] >= LIMIT_TO_DISTINCT_NUMBERS_OF_BUILDINGS)
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

void ZerGreenAI::BuildOrderManager::onFrame()
{
	if (Broodwar->getFrameCount() % 10 != 0)
	{
		return;
	}

#ifdef _DEBUG
	// Leaves the game soon after 10000 frames
	if (Broodwar->getFrameCount() >= 10000)
	{
		if (rand() % 10)
			Broodwar->leaveGame();
	}
#endif

	if (constructionFailed)
	{
		UnitType chosenAction = chooseNextAction(CHANCE_TO_RANDOMIZE_ACTION);
		
		// Deliberate use of assign operator in if statement
		if (!(constructionFailed = !ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(chosenAction)))
		{
			rememberChosenAction(chosenAction);
		}
		Broodwar << chosenAction.c_str() << " chosen" << std::endl;
	}
}

UnitType ZerGreenAI::BuildOrderManager::chooseNextAction(float chanceToChooseRandomly)
{
	if ((float)rand() / (float)RAND_MAX < chanceToChooseRandomly)
	{
		Broodwar << "Being epsilon-greedy" << std::endl;
		return protossBuildingTypesExcludingPylon[rand() % protossBuildingTypesExcludingPylon.size()];
	}
		
	float bestValue = std::numeric_limits<float>::lowest();
	UnitType bestAction = UnitTypes::None;
	for (UnitType testAction : protossBuildingTypesExcludingPylon)
	{
		std::vector <float> nnFrameData = getInput(testAction);

		/*Broodwar << "Input : ";
		for (float b : nnFrameData)
		{
			Broodwar << b << ' ';
		}
		Broodwar << std::endl;*/

		//Broodwar << "Output: ";
		std::vector <float> output;
		output = neuralNetwork.run(nnFrameData);
		/*for (float b : output)
		{
			Broodwar << b << ' ';
		}
		Broodwar << std::endl;*/
		
		if (output.front() > bestValue) // Should be exactly 1 in front
		{
			bestAction = testAction;
			bestValue = output.front();
		}
	}
	return bestAction;
}

void ZerGreenAI::BuildOrderManager::rememberChosenAction(UnitType action)
{
	for (float f : getInput(action))
	{
		gameFile << f << ", ";
	}
	gameFile << '\n';
}

void ZerGreenAI::BuildOrderManager::onUnitCreate(Unit u)
{
	if ((IsOwned && IsBuilding)(u) && u->getType() != UnitTypes::Protoss_Pylon)
	{
		numUnitsOfType[u->getType()]++;

		UnitType chosenAction = chooseNextAction(CHANCE_TO_RANDOMIZE_ACTION);

		// Deliberate use of assign operator in if statement
		if (!(constructionFailed = !ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(chosenAction)))
		{
			rememberChosenAction(chosenAction);
		}

		Broodwar << chosenAction.c_str() << " chosen" << std::endl;

	}
}

void ZerGreenAI::BuildOrderManager::onUnitMorph(Unit u)
{
	if ((IsOwned && IsRefinery)(u))
	{
		numUnitsOfType[u->getType()]++;
		onUnitCreate(u);
	}
	else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
	{
		numUnitsOfType[Broodwar->self()->getRace().getRefinery()]--;
	}

}

void ZerGreenAI::BuildOrderManager::onUnitDestroy(Unit u)
{
	if ((IsOwned && IsBuilding)(u))
	{
		numUnitsOfType[u->getType()]--;
	}
}

ZerGreenAI::BuildOrderManager::BuildOrderManager() : neuralNetwork(getNeuralNetworkInputStream())
{

}

ZerGreenAI::BuildOrderManager::~BuildOrderManager()
{
	gameFile.close();
}