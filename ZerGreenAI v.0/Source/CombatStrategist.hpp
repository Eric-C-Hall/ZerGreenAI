#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{
	class CombatStrategist : public UnitManager
	{
		void onStart() override;
		void onAssignment(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Combat Strategist"; }
		void onFrame();
	};
}