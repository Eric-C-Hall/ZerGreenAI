#pragma once

#include "MicroCombatGroup.hpp"
#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class MacroCombatManager : public UnitManager
	{
		std::unordered_set<MicroCombatManager *> childManagers;

		void newManager(BWAPI::Unit u);
		void onFrame() override;

		void onAssignment(BWAPI::Unit u);
	public:
		inline std::string name() { return "Macro Combat Manager"; }
	};

	MacroCombatManager* getMacroCombatManager();

}