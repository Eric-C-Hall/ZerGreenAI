#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ProbeScoutManager : public UnitManager
	{
		void onAssignment(BWAPI::Unit u) override;
		void onReassignment(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Probe Scout Manager"; }
		void onFrame() override;
	};

	void startProbeScout();

}