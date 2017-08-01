#pragma once

#include "Namespaces.h"

#include "bwemL.h"

struct LineSegment
{
	Position end1;
	Position end2;

	void draw(Color color);
};

void initializeMapAnalyser();

LineSegment mineralLine(const BWEM::Base * b);

std::vector<TilePosition> pathToTilePosition(TilePosition begin, TilePosition end);
TilePosition areaCenter(const BWEM::Area *);
Position averageWeightedPosition(std::unordered_map<Position, double> weights);
