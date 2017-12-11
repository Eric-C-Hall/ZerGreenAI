#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "ModularNN.h"


namespace ZerGreenAI
{
	const int LIMIT_TO_DISTINCT_NUMBERS_OF_BUILDINGS = 1;

	extern std::vector<BWAPI::UnitType> protossBuildingTypes;

	class BuildOrderManager : Manager
	{
		static const int NUM_REMEMBER_GAMES = 10000;
		static const float LEARNING_SPEED;
		static const float CHANCE_TO_RANDOMIZE_ACTION;
		ModularNN neuralNetwork;
		int gameNumber;
		std::ofstream gameFile;
		bool constructionFailed = false;

		std::vector<std::string> getFileLabelledInput(std::string fileName);
		std::string getSaveFileName(int i);
		std::map<BWAPI::UnitType, int> numUnitsOfType;
		UnitType chooseNextAction(float chanceToChooseRandomly);
		void rememberChosenAction(UnitType action);

		void onStart() override;
		void onEnd(bool didWin) override;
		void onFrame() override;
		void onUnitCreate(Unit u) override;
		void onUnitMorph(Unit u) override;
		void onUnitDestroy(Unit u) override;

		std::vector<float> getInput(UnitType whatAction);
	public:
		BuildOrderManager();
		~BuildOrderManager();
	};

	enum AttributeType
	{
		enemyRace,
		allyUnit,
		enemyUnit
	};

	struct UnitData
	{
		BWAPI::UnitType type;
		int number;
	};

	union AttributeData
	{
		BWAPI::Race race;
		UnitData unitData;
	};

	class Attribute
	{
		AttributeType type;
		AttributeData data;
	};
}