#pragma once

#include "GeneralManagement.hpp"

namespace ZerGreenAI
{
	class UnitManager : public Manager
	{
	protected:
		// Important Note:
		// UnitManager::onAssignment, etc should be called in the relevant methods of any derived class.
		virtual void onAssignment(BWAPI::Unit u) {};
		virtual void onReassignment(BWAPI::Unit u) {};
		virtual void onUnitDestroy(BWAPI::Unit u) override;
		virtual void onUnitRenegade(BWAPI::Unit u) override;
		virtual void onFrame();

		BWAPI::Unitset assignedUnits;
		void giveUnitManagement(BWAPI::Unit u, UnitManager * const mgr);

	public:
		virtual ~UnitManager();
		inline virtual std::string name() { return "Unnamed Unit Manager"; }

		bool giveOrphanUnit(BWAPI::Unit u);
	};

	UnitManager* getUnitManager(BWAPI::Unit u);
}