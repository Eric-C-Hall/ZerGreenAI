#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"

#define USING_N00BY_EDGE true

#if USING_N00BY_EDGE
#include "ModularNN.h"
#endif


namespace ZerGreenAI
{
	class Brain;
	class ConstructionBrain;

	template<int type>
	class ProductionBrain;
	
	class BrainManager : Manager
	{
		static const int NUM_REMEMBER_GAMES = 10000;
		static const int MAX_NUM_FRAMES = 30000;

		int gameNumber;
		std::vector<Brain *> brains;
		bool didTie = false;
		bool ignoreResults = false;
		int numAllyResourcesLost = 0;
		int numEnemyResourcesLost = 0;

		static inline std::string getGamenumberFileName();

		void onStart() override;
		void onFrame() override;
		void onUnitDestroy(BWAPI::Unit u) override;
		void onEnd(bool didWin) override;
		void onSendText(std::string text) override;
		void onReceiveText(BWAPI::Player player, std::string text) override;
	public:
		//ConstructionBrain * constructionBrain;
		ProductionBrain<BWAPI::UnitTypes::Protoss_Gateway> * gatewayBrain;
		ProductionBrain<BWAPI::UnitTypes::Protoss_Stargate> * stargateBrain;
		ProductionBrain<BWAPI::UnitTypes::Protoss_Robotics_Facility> * roboFacBrain;

		BrainManager();
		~BrainManager();

		inline int getGameNumber() { return gameNumber; }

		std::string name() override { return "Brain Manager"; }

		friend Brain;
	};

#if USING_RLLIB
	typedef std::vector<float> S;
	typedef BWAPI::UnitType A;
#endif

	class Brain
	{
		const float LEARNING_SPEED;
		const float CHANCE_TO_RANDOMIZE_ACTION;
		
		std::ofstream gameFile;

		std::vector<std::string> getFileLabelledInput(std::string fileName);
		inline std::string getSaveFileName(int i);
		inline std::string ZerGreenAI::Brain::getNNFileName();
		inline std::ifstream getNeuralNetworkInputStream();
		inline std::ofstream getNeuralNetworkOutputStream();
		
		void useResults(double gameValue);
		void initializeBrain();
		void rememberChosenAction(BWAPI::UnitType action);

		virtual std::vector<float> getInput(BWAPI::UnitType whatAction) = 0;
		virtual std::vector<BWAPI::UnitType> getPossibleActions() = 0;
		virtual void onStart() {}

#if USING_N00BY_EDGE
		ModularNN * neuralNetwork;
#endif

	protected:
		Brain(float learningSpeed, float randomizeActionChance);

	public:
		UnitType choose();
		
		virtual std::string getSafeName() { return "ERROR_NO_SAFE_NAME"; } // getSafeName returns the name that is used in filepaths for files related to the brain
		virtual std::string getName() { return "Unnamed Brain"; }
		
#if _DEBUG
		std::string safeName;
		std::string name = getName();
#endif

		~Brain();

		friend BrainManager;
	};
}

inline std::string ZerGreenAI::BrainManager::getGamenumberFileName()
{
	return "bwapi-data/read/instance" + std::to_string(ZerGreenAIObj::mainInstance->getExternalInstanceNumber()) + "/gameinfo/gamenumber.txt";
}

inline std::string ZerGreenAI::Brain::getSaveFileName(int i)
{
	return "bwapi-data/read/instance" + std::to_string(ZerGreenAIObj::mainInstance->getExternalInstanceNumber()) + "/gameinfo/" + getSafeName() + "/game" + std::to_string(i) + ".bo";
}

inline std::string ZerGreenAI::Brain::getNNFileName()
{
	return "bwapi-data/read/instance" + std::to_string(ZerGreenAIObj::mainInstance->getExternalInstanceNumber()) + "/neuralNetworks/" + getSafeName() +".txt";
}

std::ifstream ZerGreenAI::Brain::getNeuralNetworkInputStream()
{
	std::ifstream model(getNNFileName());
	assert(model.is_open());
	return model;
}

std::ofstream ZerGreenAI::Brain::getNeuralNetworkOutputStream()
{
	std::ofstream model(getNNFileName());
	assert(model.is_open());
	return model;
}