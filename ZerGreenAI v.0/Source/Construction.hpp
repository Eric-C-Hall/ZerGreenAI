#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ConstructionManager : public UnitManager
	{
		bool acceptRequest(BWAPI::Unit u); // Preferably should blacklist rather than whitelist
		void recycleUnit(BWAPI::Unit u);

		void onFrame();
		void onUnitCreate(Unit u) override;
		void onUnitMorph(Unit u) override;
	public:
		inline virtual std::string name() { return "Construction Manager"; }

		bool constructBuilding(BWAPI::UnitType type);
	};

}