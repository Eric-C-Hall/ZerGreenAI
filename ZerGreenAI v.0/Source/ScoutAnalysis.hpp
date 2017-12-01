#pragma once

#include "bwemL.hpp"

namespace ZerGreenAI
{

	void scoutAnalysisOnDiscover(BWAPI::Unit u);

	// int is priority, higher is higher priority
	std::map<const BWEM::Area *, int> getTargets();
	const BWEM::Area * getHighestPriorityTarget();

}