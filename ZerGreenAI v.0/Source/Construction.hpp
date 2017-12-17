#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ConstructionManager : public UnitManager
	{
		bool acceptRequest(BWAPI::Unit u); // Preferably should blacklist rather than whitelist

		void onFrame() override;
		void onUnitCreate(BWAPI::Unit u) override;
		void onUnitMorph(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Construction Manager"; }

		bool isMakingBuilding(BWAPI::UnitType t);
		bool constructBuilding(BWAPI::UnitType type);
	};

}