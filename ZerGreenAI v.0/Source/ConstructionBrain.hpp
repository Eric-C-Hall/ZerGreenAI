#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "Brain.hpp"


namespace ZerGreenAI
{
	const int LIMIT_TO_DISTINCT_NUMBERS_OF_BUILDINGS = 5;


	class ConstructionBrain : public Brain
	{
		static const std::vector<BWAPI::UnitType> protossBuildingTypesExcludingPylon;

		bool actionIsValid(UnitType action);

		std::vector<UnitType> getPossibleActions() override;
		std::vector<float> getInput(UnitType whatAction) override;
		void onStart() override;

	public:
		bool displayPossibleUnits = false;

		void drawDebug();

		std::string getSafeName() override { return "construction"; }
		std::string getName() override { return "Construction Brain"; }
		ConstructionBrain();
	};
}