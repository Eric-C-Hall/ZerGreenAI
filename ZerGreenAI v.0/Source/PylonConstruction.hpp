#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "ModularNN.h"

namespace ZerGreenAI
{

	class PylonConstructionManager : public Manager
	{
		void onFrame() override;
		void onUnitCreate(BWAPI::Unit u) override;
		void onUnitDestroy(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Pylon Construction Manager"; }
	};

}