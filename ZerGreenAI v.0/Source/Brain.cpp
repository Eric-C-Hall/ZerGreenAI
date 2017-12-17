#include "stdafx.h"

#include "Brain.hpp"

#include "ConstructionBrain.hpp"
#include "ProductionBrain.hpp"



void ZerGreenAI::BrainManager::onStart()
{
	didTie = false;
	ignoreResults = false;

	//constructionBrain = new ConstructionBrain;
	gatewayBrain = new ProductionBrain<UnitTypes::Protoss_Gateway>;
	stargateBrain = new ProductionBrain<UnitTypes::Protoss_Stargate>;
	roboFacBrain = new ProductionBrain<UnitTypes::Protoss_Robotics_Facility>;

	for (Brain * brain : brains)
	{
		brain->initializeBrain();
		brain->onStart();
	}

}

void ZerGreenAI::BrainManager::onFrame()
{
	// Leaves the game with a tie after 30000 frames. Should remove this when not testing
	if (Broodwar->getFrameCount() >= MAX_NUM_FRAMES)
	{
		didTie = true;
		Broodwar->leaveGame();
	}

	//constructionBrain->drawDebug();
}

void ZerGreenAI::BrainManager::onUnitDestroy(BWAPI::Unit u)
{
	if (IsAlly(u))
	{
		numAllyResourcesLost += u->getType().mineralPrice();
		numAllyResourcesLost += u->getType().gasPrice();
	}
	else
	{
		numEnemyResourcesLost += u->getType().mineralPrice();
		numEnemyResourcesLost += u->getType().gasPrice();
	}
}

void ZerGreenAI::BrainManager::onEnd(bool didWin)
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

		/*// Could potentially be ratio of their units killed to our units killed, but perhaps due to fog of war, efficiency could be inaccurate.
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
		}*/

		if (numAllyResourcesLost + numEnemyResourcesLost != 0)
		{
			gameValue += ((float)numEnemyResourcesLost / (float)(numAllyResourcesLost + numEnemyResourcesLost)) * 0.2f;
		}
		else
		{
			gameValue += 0.1f;
		}
		assert(gameValue >= 0.4 && gameValue <= 0.6);
	}
	else
	{
		gameValue = didWin;

		// Should be between 0 and 0.5, increasing as game length increases.
		float gameLengthValue = ((float)Broodwar->getFrameCount() / (float)MAX_NUM_FRAMES) * 0.4f;
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

	for (Brain * brain : brains)
	{
		brain->useResults(gameValue);
	}
}

void ZerGreenAI::BrainManager::onSendText(std::string text)
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
	else if (text == "possibleUnits")
	{
		Broodwar << "Displaying possible Units" << std::endl;
		//constructionBrain->displayPossibleUnits = !constructionBrain->displayPossibleUnits;
	}
}

void ZerGreenAI::BrainManager::onReceiveText(BWAPI::Player player, std::string text)
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
	else if (text == "possibleUnits")
	{
		Broodwar << "Displaying possible Units" << std::endl;
		//constructionBrain->displayPossibleUnits = !constructionBrain->displayPossibleUnits;
	}
}

ZerGreenAI::BrainManager::BrainManager()
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

	std::ofstream gameNumberFileOutput(getGamenumberFileName(), ofstream::out | ofstream::trunc);
	assert(gameNumberFileOutput.is_open());
	gameNumberFileOutput << gameNumber;
	gameNumberFileOutput.close();
}

ZerGreenAI::BrainManager::~BrainManager()
{
	for (Brain * brain : brains)
	{
		delete brain;
	}
}

void ZerGreenAI::Brain::useResults(double gameValue)
{
	gameFile << gameValue << std::endl;;

	for (const std::string &experienceString : getFileLabelledInput(getSaveFileName(ZerGreenAIObj::mainInstance->brainManager->getGameNumber())))
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

#if USING_N00BY_EDGE
		neuralNetwork->updateNeuralNetwork(experienceVector, { value }, LEARNING_SPEED);
		neuralNetwork->write(getNeuralNetworkOutputStream());
#endif
	}
}

void ZerGreenAI::Brain::initializeBrain()
{
#if _DEBUG
	safeName = getSafeName();
#endif

#if USING_N00BY_EDGE
	neuralNetwork = new ModularNN(getNeuralNetworkInputStream());
#endif	

	for (int i = 0; i < ZerGreenAIObj::mainInstance->brainManager->getGameNumber(); i++)
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

	gameFile.open(getSaveFileName(ZerGreenAIObj::mainInstance->brainManager->getGameNumber()));
}

void ZerGreenAI::Brain::rememberChosenAction(UnitType action)
{
	for (float f : getInput(action))
	{
		gameFile << f << ", ";
	}
	gameFile << '\n';
}

UnitType ZerGreenAI::Brain::choose()
{
	auto possibleActions = getPossibleActions();
	if (possibleActions.size() > 0)
	{
		if ((float)rand() / (float)RAND_MAX < CHANCE_TO_RANDOMIZE_ACTION)
		{
			return possibleActions[rand() % possibleActions.size()];
		}
		float bestValue = std::numeric_limits<float>::lowest();
		UnitType bestAction = UnitTypes::None;
		for (UnitType testAction : possibleActions)
		{
			std::vector <float> nnFrameData = getInput(testAction);
			std::vector <float> output;
#if USING_N00BY_EDGE
			output = neuralNetwork->run(nnFrameData);
#endif
			if (output.front() > bestValue)
			{
				bestAction = testAction;
				bestValue = output.front();
			}
		}
		rememberChosenAction(bestAction);
		return bestAction;
	}
	else
	{
		return UnitTypes::None;
	}
}

std::vector<std::string> ZerGreenAI::Brain::getFileLabelledInput(std::string fileName)
{
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
}

ZerGreenAI::Brain::~Brain()
{
#if USING_N00BY_EDGE
	delete neuralNetwork;
#endif
	gameFile.close();
}


ZerGreenAI::Brain::Brain(float learningSpeed, float randomizeActionChance) : LEARNING_SPEED(learningSpeed), CHANCE_TO_RANDOMIZE_ACTION(randomizeActionChance)	
{
	ZerGreenAIObj::mainInstance->brainManager->brains.push_back(this);
};