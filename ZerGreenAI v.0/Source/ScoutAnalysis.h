#pragma once

#include "Namespaces.h"
#include "bwemL.h"

void scoutAnalysisOnDiscover(Unit u);

// int is priority, higher is higher priority
std::map<const BWEM::Area *, int> getTargets();
const BWEM::Area * getHighestPriorityTarget();