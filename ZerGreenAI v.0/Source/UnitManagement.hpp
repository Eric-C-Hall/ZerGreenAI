#pragma once

#include "GeneralManagement.hpp"

namespace ZerGreenAI
{
	class UnitManager : public Manager
	{
		virtual void onAssignment(BWAPI::Unit u) {};
		virtual void onReassignment(BWAPI::Unit u) {};
		void onUnitDestroy(BWAPI::Unit u) override;
		void onUnitRenegade(BWAPI::Unit u) override;
		void onFrame();

	protected:
		BWAPI::Unitset assignedUnits;
		void giveUnitManagement(BWAPI::Unit u, UnitManager * const mgr);

	public:
		virtual ~UnitManager();
		inline virtual std::string name() { return "Unnamed Unit Manager"; }

		bool giveOrphanUnit(BWAPI::Unit u);
	};

	UnitManager* getUnitManager(BWAPI::Unit u);
}