#pragma once

#include "QueueUnitManagement.hpp"

namespace ZerGreenAI
{

#define MCG_LEASH 500
#define MCG_PUSH_RADIUS 200

	class MicroCombatManager : public QueueUnitManager
	{
		std::vector<Position> path; // Path from center to target
		BWAPI::Position center;
		BWAPI::Position target;
		void onUnitTurn(BWAPI::Unit u) override;
		void recycleUnit(BWAPI::Unit u) override;
	public:
		inline MicroCombatManager(BWAPI::Position center, BWAPI::Position target) : center(center) { updateTarget(target); }
		inline BWAPI::Position getCenter() { return center; }
		inline virtual std::string name() { return "Micro Combat Manager"; }
		void onFrame();
		void absorb(MicroCombatManager * other);
		void updateTarget(BWAPI::Position p);
		inline BWAPI::Position getTarget() { return target; }
		~MicroCombatManager();
	};

}