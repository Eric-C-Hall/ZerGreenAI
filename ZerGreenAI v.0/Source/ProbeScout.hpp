#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ProbeScoutManager : public UnitManager
	{
		void recycleUnit(BWAPI::Unit u);
	public:
		inline virtual std::string name() { return "Probe Scout Manager"; }
		ProbeScoutManager(BWAPI::Unit u);
		void onFrame();
	};

	void startProbeScout();

}