#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ConstructionManager : public UnitManager
	{
		bool acceptRequest(BWAPI::Unit u); // Preferably should blacklist rather than whitelist
		void recycleUnit(BWAPI::Unit u);
	public:
		inline virtual std::string name() { return "Construction Manager"; }
		void onFrame();
		void onUnitCreate(Unit u) override;
		void onUnitMorph(Unit u) override;

		bool constructBuilding(BWAPI::UnitType type);
	};

}