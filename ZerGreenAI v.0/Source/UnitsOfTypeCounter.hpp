#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "ZerGreenAI.hpp"
#include "Construction.hpp"

namespace ZerGreenAI
{

	class UnitsOfTypeCounter : public Manager
	{
		std::unordered_map<UnitType, int> numUnitsOfTypeMap;

		void onUnitCreate(BWAPI::Unit u) override;
		void onUnitDestroy(BWAPI::Unit u) override;
		void onUnitMorph(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Units of Type Counter"; }
		inline int numUnitsOfType(BWAPI::UnitType type) { return numUnitsOfTypeMap[type] + ZerGreenAIObj::mainInstance->constructionManager->isMakingBuilding(type); }
	};

}