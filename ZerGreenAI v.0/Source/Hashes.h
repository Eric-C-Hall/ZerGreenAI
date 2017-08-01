#pragma once

#include "Namespaces.h"

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
	struct hash<UnitType>
	{
		size_t operator()(UnitType type) const
		{
			return type.getID();
		}
	};

	template<>
	struct hash<UpgradeType>
	{
		size_t operator()(UpgradeType type) const
		{
			return type.getID();
		}
	};

	template<>
	struct hash<TechType>
	{
		size_t operator()(TechType type) const
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