#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vector.hpp"
#include "TriangularGrid.hpp"

template<int distance>
void ZerGreenAI::TriangularGrid<distance>::addNode(BWAPI::Position p)
{
	for (int negate = 0; negate < 2; negate++)
	{
		for (BWAPI::Position pos : {southEast, southWest, east})
		{
			if (negate != 0)
				pos *= -1;

			BWAPI::Position currentNode = p + pos;

			if (BWAPI::Broodwar->hasPath(p, currentNode) && BWAPI::Broodwar->hasPath(p, (p + currentNode) / 2))
			{
				grid[p].insert(currentNode);
				if (grid.count(currentNode) == 0)
				{
					addNode(currentNode);
				}
			}
		}
	}
}

template<int distance>
std::vector<BWAPI::Position> ZerGreenAI::TriangularGrid<distance>::findPath(BWAPI::Position a, BWAPI::Position b)
{
	if (!isOnGrid(a))
		a = snapToGrid(a);
	if (!isOnGrid(b))
		b = snapToGrid(b);

	std::vector<>

	return std::vector<BWAPI::Position>();
}

template<int distance>
BWAPI::Position ZerGreenAI::TriangularGrid<distance>::snapToGrid(const BWAPI::Position & p)
{
	BWAPI::Position relToStart = p - (Position)Broodwar->self()->getStartLocation();
	int numSouthEastsToP = relToStart.y / southEast.y;
	BWAPI::Position remainder = relToStart - (southEast * numSouthEastsToP);

	//Broodwar->drawCircleMap(remainder + (Position)Broodwar->self()->getStartLocation(), 10, Colors::Blue, true);


	if (remainder.y > southEast.y / 2)
	{
		numSouthEastsToP++;
	}
	else if (remainder.y < -southEast.y / 2)
	{
		numSouthEastsToP--;
	}
	remainder.x -= remainder.y * 2 / 3;
	remainder.y = 0;

	//Broodwar->drawCircleMap(remainder + (Position)Broodwar->self()->getStartLocation(), 10, Colors::Purple, true);

	int numEastsToP = remainder.x / east.x;
	BWAPI::Position secondRemainder = remainder - (east * numEastsToP);
	//Broodwar->drawCircleMap(secondRemainder + (Position)Broodwar->self()->getStartLocation(),10,Colors::White,true );

	if (secondRemainder.x > east.x / 2)
	{
		numEastsToP++;
	}
	else if (secondRemainder.x < -east.x / 2)
	{
		numEastsToP--;
	}

	return southEast * numSouthEastsToP + east * numEastsToP + (Position)Broodwar->self()->getStartLocation();
}

template<int distance>
void ZerGreenAI::TriangularGrid<distance>::draw(BWAPI::Color color)
{
	for (const auto &node : grid)
	{
		Broodwar->drawCircleMap(node.first, 5, color, true);
		for (const auto &neighbour : node.second)
		{
			Broodwar->drawLineMap(node.first, neighbour, color);
		}
	}
}

template <int distance>
const BWAPI::Position ZerGreenAI::TriangularGrid<distance>::southEast = getVectorPolar(M_PI / 3, distance);
template <int distance>
const BWAPI::Position ZerGreenAI::TriangularGrid<distance>::southWest = getVectorPolar(2 * M_PI / 3, distance);
template <int distance>
const BWAPI::Position ZerGreenAI::TriangularGrid<distance>::east = Position(getVectorPolar(M_PI / 3, distance).x - getVectorPolar(2 * M_PI / 3, distance).x, 0);

template<int distance>
ZerGreenAI::TriangularGrid<distance>::TriangularGrid()
{
	addNode((BWAPI::Position)BWAPI::Broodwar->self()->getStartLocation());
}