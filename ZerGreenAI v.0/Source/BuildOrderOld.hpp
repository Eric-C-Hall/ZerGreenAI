#pragma once

#include <BWAPI.h>
#include "Construction.hpp"

namespace ZerGreenAI
{

	void buildOrderOnCreate(BWAPI::Unit unit);
	void buildOrderOnRecycle(BWAPI::Unit unit);

	void buildOrderOnFrame();
	void initializeBuildOrder();

}