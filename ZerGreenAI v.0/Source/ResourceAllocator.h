#pragma once

#include "Namespaces.h"
#include "Management.h"

class ResourceAllocator : public JuniorManager
{
public:
	inline std::string name() { return "Resource Allocator"; }
	void onFrame();
};
