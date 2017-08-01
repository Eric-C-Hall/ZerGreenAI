#pragma once

#include "bwemL.h"
#include "Hashes.h"

namespace std
{
	bool operator==(const BWEM::Base &a, const BWEM::Base &b)
	{
		return a.Location() == b.Location();
	}

	template<>
	struct hash<BWEM::Area>
	{
		size_t operator()(BWEM::Area area) const
		{
			return area.Id();
		}
	};

	template<>
	struct hash<BWEM::Base>
	{
		size_t operator()(BWEM::Base base) const
		{
			return hash<BWAPI::TilePosition>()(base.Location());
		}
	};
}