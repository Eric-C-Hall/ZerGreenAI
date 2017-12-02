#pragma once

#include "QueueUnitManagement.hpp"

namespace ZerGreenAI
{

#define MCG_LEASH 500
#define MCG_PUSH_RADIUS 200

	class MicroCombatManager : public QueueUnitManager
	{
		BWAPI::Position center;
		void onUnitTurn(BWAPI::Unit u);
	public:
		inline MicroCombatManager(BWAPI::Position center, BWAPI::Position target) : center(center), target(target) {};
		BWAPI::Position target;
		inline BWAPI::Position getCenter() { return center; }
		inline std::string name() { return "Micro Combat Manager"; }
		void onFrame();
		void absorb(MicroCombatManager * other);
	};

}