#pragma once

#include <BWAPI.h>
#include <unordered_map>

#include "QueueUnitManagement.hpp"

namespace ZerGreenAI
{

	class BoringCombatManager : public QueueUnitManager
	{
		std::unordered_map<BWAPI::Unit, int> framesSinceLastTurn;
		void onUnitTurn(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Boring Combat Manager"; }
	};

}