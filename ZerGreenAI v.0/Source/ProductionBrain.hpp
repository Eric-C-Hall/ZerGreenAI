#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "Brain.hpp"

namespace ZerGreenAI
{
	template<int type>
	class ProductionBrain : public Brain
	{
		bool actionIsValid(BWAPI::UnitType action);

		std::vector<BWAPI::UnitType> getPossibleActions() override;
		std::vector<float> getInput(BWAPI::UnitType whatAction) override;
		void onStart() override;

	public:

		ProductionBrain();

		std::string getSafeName() override { return ((UnitType)type).c_str(); }
		std::string getName() override { return ((UnitType)type).toString() + " Production Brain"; }
	};
}

#include "ProductionBrain.tpp"