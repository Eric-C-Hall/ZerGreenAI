#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class CombatStrategist : public UnitManager
	{
		void onStart();
		void onAssignment(BWAPI::Unit u);
	public:
		inline virtual std::string name() { return "Combat Strategist"; }
		void onFrame();
	};

	CombatStrategist * getCombatStrategist();

}