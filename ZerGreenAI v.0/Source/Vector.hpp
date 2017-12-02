#pragma once

#include "BWAPI.h"

namespace ZerGreenAI
{
	inline BWAPI::Position getVectorPolar(double direction, int length)
	{
		return BWAPI::Position((int)(length * cos(direction)), (int)(length * sin(direction)));
	}

	inline BWAPI::Position scaleVectorToSize(BWAPI::Position p, int size)
	{
		return (p*size) / p.getApproxDistance(BWAPI::Position(0, 0));
	}

}