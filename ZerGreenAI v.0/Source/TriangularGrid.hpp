#pragma once

#include <BWAPI.h>
#include "Hashes.hpp"

namespace ZerGreenAI
{
	template<int distance>
	class TriangularGrid
	{
		std::unordered_map<BWAPI::Position, std::unordered_set<BWAPI::Position>> grid;
		static const BWAPI::Position southEast;
		static const BWAPI::Position southWest;
		static const BWAPI::Position east;

		void addNode(BWAPI::Position p);

		class GridPositionAStar;
	public:
		std::vector<BWAPI::Position> findPath(BWAPI::Position a, BWAPI::Position b);
		inline bool isOnGrid(const BWAPI::Position &p) { return grid.count(p); }
		BWAPI::Position snapToGrid(const BWAPI::Position &p);
		void draw(BWAPI::Color color);
		TriangularGrid();
	};
}

#include "TriangularGrid.tpp"