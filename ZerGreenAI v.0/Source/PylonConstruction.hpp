#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "ModularNN.h"

namespace ZerGreenAI
{

	class PylonConstructionManager : public Manager
	{
		void onFrame() override;
	public:
		inline virtual std::string name() { return "Pylon Construction Manager"; }
	};

}