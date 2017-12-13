#pragma once

#include <queue>

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class QueueUnitManager : public UnitManager
	{
		std::deque<BWAPI::Unit> unitDecideQueue; // There should be no reason to need unitDecideQueue outside of QueueUnitManagement: Use assignedUnits instead
		BWAPI::Unit rotateQueue();
		virtual void onUnitTurn(BWAPI::Unit u) {};
	protected:
		// Important Note:
		// QueueUnitManager::onAssignment, etc should be called in the relevant methods of any derived class.
		virtual void onFrame() override;
		virtual void onAssignment(BWAPI::Unit u) override;
		virtual void onReassignment(BWAPI::Unit u) override;

		int turnLength();
	public:
		inline virtual std::string name() { return "Unnamed Queue Unit Manager"; }
	};

}