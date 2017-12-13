#include "stdafx.h"

#include <sstream>

#include "ZerGreenAI.hpp"
#include "Construction.hpp"
#include "BuildOrder.hpp"
#include "ModularNN.h"
#include "UnitsOfTypeCounter.hpp"

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
	didTie = false;
	ignoreResults = false;

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
	if (ignoreResults)
		return;

	float gameValue;

	// GameValue is between 0 and 1.
	// A loss is between 0 and 0.4
	// A tie is between 0.4 and 0.6
	// A win is between 0.6 and 1

	if (didTie)
	{
		gameValue = 0.4f;

		// Could potentially be ratio of their units killed to our units killed, but perhaps due to fog of war, efficiency could be inaccurate.
		// This probably works fine.
		int gameResourcesValue = Broodwar->self()->gatheredMinerals() + 2 * Broodwar->self()->gatheredGas();
		int maxNumberResources = 0;
		for (Unit u : Broodwar->getStaticMinerals())
		{
			maxNumberResources += u->getInitialResources();
		}
		for (Unit u : Broodwar->getStaticGeysers())
		{
			maxNumberResources += u->getInitialResources();
		}

		gameValue += ((float)gameResourcesValue / (float)maxNumberResources) * 0.2f;
		assert(gameValue >= 0.4 && gameValue <= 0.6);
	}
	else
	{
		gameValue = didWin;

		// Should be between 0 and 0.5, increasing as game length increases.
		float gameLengthValue = ((float)Broodwar->getFrameCount() /(float)MAX_NUM_FRAMES ) * 0.4f;
		if (didWin)
		{
			gameValue -= gameLengthValue;
			assert(gameValue >= 0.6 && gameValue <= 1);
		}
		else
		{
			gameValue += gameLengthValue;
			assert(gameValue >= 0 && gameValue <= 0.4);
		}
	}

	gameFile << gameValue << std::endl;;

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

	// Leaves the game with a tie after 30000 frames. Should remove this when not testing
	if (Broodwar->getFrameCount() >= MAX_NUM_FRAMES)
	{
		didTie = true;
		Broodwar->leaveGame();
	}

	if (constructionFailed)
	{
		constructionFailed = !ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(lastChosenAction);
	}
}

UnitType ZerGreenAI::BuildOrderManager::chooseNextAction(float chanceToChooseRandomly)
{
	std::vector<UnitType> possibleActions;
	for (auto type : protossBuildingTypesExcludingPylon)
	{
		if (actionIsValid(type))
		{
			possibleActions.push_back(type);
		}
	}

	assert(possibleActions.size() > 0);

	if ((float)rand() / (float)RAND_MAX < chanceToChooseRandomly)
	{
		Broodwar << "Being epsilon-greedy" << std::endl;
		return possibleActions[rand() % possibleActions.size()];
	}
		
	float bestValue = std::numeric_limits<float>::lowest();
	UnitType bestAction = UnitTypes::None;
	for (UnitType testAction : possibleActions)
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

bool ZerGreenAI::BuildOrderManager::actionIsValid(UnitType action)
{
	for (auto reqType : action.requiredUnits())
	{
		if (reqType.first == UnitTypes::Protoss_Pylon || reqType.first == UnitTypes::Protoss_Probe)
			continue;

		if (ZerGreenAIObj::mainInstance->unitsOfTypeCounter->numUnitsOfType(reqType.first) < reqType.second)
		{
			return false;
		}
	}

	return true;
}

void ZerGreenAI::BuildOrderManager::onUnitCreate(Unit u)
{
	if ((IsOwned && IsBuilding)(u) && u->getType() != UnitTypes::Protoss_Pylon)
	{
		numUnitsOfType[u->getType()]++;

		UnitType chosenAction = chooseNextAction(CHANCE_TO_RANDOMIZE_ACTION);
		rememberChosenAction(chosenAction);
		Broodwar << chosenAction.c_str() << " chosen" << std::endl;

		lastChosenAction = chosenAction;
		constructionFailed = !ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(chosenAction);

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

void ZerGreenAI::BuildOrderManager::onSendText(std::string text)
{
	if (text == "tie")
	{
		didTie = true;
		Broodwar->leaveGame();
	}
	else if (text == "loss")
	{
		Broodwar->leaveGame();
	}
	else if (text == "void")
	{
		ignoreResults = true;
		Broodwar->leaveGame();
	}
	else if (text == "nolearn")
	{
		ignoreResults = true;
		Broodwar << "Build order learning cancelled for this game" << std::endl;
	}
}

void ZerGreenAI::BuildOrderManager::onReceiveText(BWAPI::Player player, std::string text)
{
	if (text == "tie")
	{
		didTie = true;
		Broodwar->leaveGame();
	}
	else if (text == "void")
	{
		ignoreResults = true;
		Broodwar->leaveGame();
	}
	else if (text == "nolearn")
	{
		ignoreResults = true;
		Broodwar << "Build order learning cancelled for this game" << std::endl;
	}
}

ZerGreenAI::BuildOrderManager::BuildOrderManager() : neuralNetwork(getNeuralNetworkInputStream())
{

}

ZerGreenAI::BuildOrderManager::~BuildOrderManager()
{
	gameFile.close();
}