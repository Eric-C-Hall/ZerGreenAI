#pragma once

#include "GeneralManagement.hpp"

namespace ZerGreenAI
{

	class UnitManager : public Manager
	{
		virtual void onAssignment(BWAPI::Unit u) {};
		virtual bool acceptRequest(BWAPI::Unit u) { return true; }; // Preferably should blacklist rather than whitelist
		virtual void recycleUnit(BWAPI::Unit u) {};
		void onFrame();

	protected:
		BWAPI::Unitset assignedUnits;
		void giveUnitManagement(BWAPI::Unit u, UnitManager * const mgr);
		bool requestUnitManagement(BWAPI::Unit u);

	public:
		virtual ~UnitManager();
		inline virtual std::string name() { return "Unnamed Unit Manager"; }

		bool giveOrphanUnit(BWAPI::Unit u);
		void recycleUnitJunior(BWAPI::Unit u);
	};

	UnitManager* getUnitManager(BWAPI::Unit u);
	void recycleUnitSenior(BWAPI::Unit u);
}