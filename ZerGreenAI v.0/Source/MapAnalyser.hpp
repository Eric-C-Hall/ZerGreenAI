#pragma once

#include "bwemL.hpp"

namespace ZerGreenAI
{

	struct LineSegment
	{
		BWAPI::Position end1;
		BWAPI::Position end2;

		void draw(BWAPI::Color color);
	};

	void initializeMapAnalyser();

	LineSegment mineralLine(const BWEM::Base * b);

	std::vector<BWAPI::TilePosition> pathToTilePosition(BWAPI::TilePosition begin, BWAPI::TilePosition end);
	BWAPI::TilePosition areaCenter(const BWEM::Area *);
	BWAPI::Position averageWeightedPosition(std::unordered_map<BWAPI::Position, double> weights);

}