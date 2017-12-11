#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{
	class MicroCombatManager;

	class MacroCombatManager : public UnitManager
	{
		std::unordered_set<MicroCombatManager *> childManagers;

		void newManager(BWAPI::Unit u);
		void onFrame() override;

		void onAssignment(BWAPI::Unit u);

	public:
		~MacroCombatManager();

		inline virtual std::string name() { return "Macro Combat Manager"; }
		friend MicroCombatManager;
	};
}