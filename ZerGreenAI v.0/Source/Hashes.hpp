#pragma once

#include <BWAPI.h>

namespace std
{
	template<>
	struct hash<BWAPI::Race>
	{
		size_t operator()(BWAPI::Race r) const
		{
			return r;
		}
	};

	template<>
	struct hash<BWAPI::UnitType>
	{
		size_t operator()(BWAPI::UnitType type) const
		{
			return type.getID();
		}
	};

	template<>
	struct hash<BWAPI::UpgradeType>
	{
		size_t operator()(BWAPI::UpgradeType type) const
		{
			return type.getID();
		}
	};

	template<>
	struct hash<BWAPI::TechType>
	{
		size_t operator()(BWAPI::TechType type) const
		{
			return type.getID();
		}
	};

	template<>
	struct hash<BWAPI::Position>
	{
		size_t operator()(BWAPI::Position p) const
		{
			return p.x + 7919 * p.y;
		}
	};

	template<>
	struct hash<BWAPI::TilePosition>
	{
		size_t operator()(BWAPI::TilePosition p) const
		{
			return p.x + 7919 * p.y;
		}
	};
}