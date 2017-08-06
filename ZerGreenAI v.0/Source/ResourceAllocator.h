#pragma once

#include "Namespaces.h"
#include "UnitManagement.h"

class ResourceAllocator : public UnitManager
{
public:
	inline std::string name() { return "Resource Allocator"; }
	void onFrame();
};
