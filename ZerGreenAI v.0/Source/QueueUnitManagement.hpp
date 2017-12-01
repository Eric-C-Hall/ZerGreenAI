#pragma once

#include <queue>

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class QueueUnitManager : public virtual UnitManager
	{
		std::deque<BWAPI::Unit> unitDecideQueue;	// There should be no reason to need unitDecideQueue outside of QueueUnitManagement: Use assignedUnits instead
		BWAPI::Unit rotateQueue();
		virtual void onAssignment(BWAPI::Unit u) override;
		virtual void recycleUnit(BWAPI::Unit u) override;
		virtual void onUnitTurn(BWAPI::Unit u) {};
		virtual void onFrame() override;
	public:
		inline virtual std::string name() { return "Unnamed Queue Unit Manager"; }
	};

}