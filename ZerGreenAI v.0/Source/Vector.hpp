#pragma once

#include "BWAPI.h"

namespace ZerGreenAI
{

	inline BWAPI::Position unitVector(BWAPI::Position p)
	{
		return p / p.getApproxDistance(BWAPI::Position(0, 0));
	}

}