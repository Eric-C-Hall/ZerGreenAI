#pragma once

#include <BWAPI.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vector.hpp"
#include "astar-algorithm-cpp-master\cpp\stlastar.h"
#include "Debug.hpp"

template <int distance>
class TriangularGrid<distance>::GridPositionAStar
{
	BWAPI::Position pos;

public:
	inline GridPositionAStar() : pos(BWAPI::Positions::Unknown) {}
	inline GridPositionAStar(BWAPI::Position pos) : pos(ZerGreenAIObj::mainInstance->grid->snapToGrid(pos)) {}
	float GoalDistanceEstimate(GridPositionAStar &nodeGoal) { return (float)pos.getDistance(nodeGoal.pos); }
	inline bool IsGoal(GridPositionAStar &nodeGoal) { return pos == nodeGoal.pos; }
	inline bool GetSuccessors(AStarSearch<GridPositionAStar> *astarsearch, GridPositionAStar *parent_node)
	{
		if (ZerGreenAIObj::mainInstance->grid->grid.count(pos) == 0)
		{
			Broodwar << "in AStar GetSuccessors, Pos is not on grid" << std::endl;
			return true;
		}

		for (const BWAPI::Position & successor : ZerGreenAIObj::mainInstance->grid->grid.at(pos))
		{
			astarsearch->AddSuccessor(GridPositionAStar(successor));
		}
		return true;
	}
	float GetCost(GridPositionAStar &successor) { return 1.0f; }
	inline bool IsSameState(GridPositionAStar &rhs) { return pos == rhs.pos; }
	inline BWAPI::Position getPos() { return pos; }
};

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

			bool noPathToSomeSixteenth = false;
			for (int i = 1; i <= 16; i++)
			{
				WalkPosition foo = (WalkPosition)((p*i + currentNode*(16 - i)) / 16);
				if (foo.isValid() && !BWAPI::Broodwar->isWalkable(foo))
					noPathToSomeSixteenth = true;
			}
			if (!noPathToSomeSixteenth && currentNode.isValid())
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
	std::vector<BWAPI::Position> returnValue;

	a = snapToGrid(a);
	b = snapToGrid(b);

	if (!isOnGrid(a) || !isOnGrid(b))
		return returnValue;

	AStarSearch<GridPositionAStar> astarsearch;
	astarsearch.SetStartAndGoalStates(GridPositionAStar(a), GridPositionAStar(b));
	unsigned int SearchState;

	do
	{
		SearchState = astarsearch.SearchStep();
	}
	while (SearchState == AStarSearch<GridPositionAStar>::SEARCH_STATE_SEARCHING);

	if (SearchState == AStarSearch<GridPositionAStar>::SEARCH_STATE_SUCCEEDED)
	{
		GridPositionAStar *node = astarsearch.GetSolutionStart();

		while (true)
		{
			node = astarsearch.GetSolutionNext();

			if (!node)
			{
				break;
			}
			else
			{
				returnValue.insert(returnValue.end(), node->getPos());
			}
		};
		astarsearch.FreeSolutionNodes();
	}
	else if (SearchState == AStarSearch<GridPositionAStar>::SEARCH_STATE_FAILED)
	{
		Broodwar << "Failed to find a path from " << a << " to " << b << std::endl;
	}
	else if (SearchState == AStarSearch<GridPositionAStar>::SEARCH_STATE_OUT_OF_MEMORY)
	{
		Broodwar << Text::BrightRed << "Astar search out of memory" << std::endl;
	}

	return returnValue;
}

template<int distance>
BWAPI::Position ZerGreenAI::TriangularGrid<distance>::snapToGrid(const BWAPI::Position & p)
{
	if (isOnGrid(p))
		return p;

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
		//Broodwar->drawCircleMap(node.first, 5, color, true);
		for (const auto &neighbour : node.second)
		{
			Broodwar->drawLineMap(node.first, (node.first + node.first + neighbour) / 3, color);
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