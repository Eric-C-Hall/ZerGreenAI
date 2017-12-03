#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ResourceAllocator : public UnitManager
	{
	public:
		inline virtual std::string name() { return "Resource Allocator"; }
		void onFrame() override;
	};

}