#include "MapAnalyser.h"
#include "BWEMHashes.h"

std::unordered_map<const BWEM::Base *, LineSegment> mineralLines;

bool isInit;

void LineSegment::draw(Color color)
{
	Broodwar->drawLineMap(end1, end2, color);
}

void calculateMineralLine(const BWEM::Base * b)
{
	Position arbitraryMineral = b->Minerals().front()->Pos();

	Position bestPos1 = arbitraryMineral;
	double bestDist = 0;
	for (auto const &m : b->Minerals())
	{
		double currDist = m->Pos().getDistance(arbitraryMineral);
		if (currDist > bestDist)
		{
			bestDist = currDist;
			bestPos1 = m->Pos();
		}
	}

	Position bestPos2 = arbitraryMineral;
	bestDist = 0;
	for (auto const &m : b->Minerals())
	{
		double currDist = m->Pos().getDistance(bestPos1);
		if (currDist > bestDist)
		{
			bestDist = currDist;
			bestPos2 = m->Pos();
		}
	}

	mineralLines[b].end1 = bestPos1;
	mineralLines[b].end2 = bestPos2;
}


// https://www.varsitytutors.com/hotmath/hotmath_help/topics/line-of-best-fit
void OLDcalculateMineralLine(const BWEM::Base * b)
{
	int xTotal = 0;
	int xNum = 0;
	int yTotal = 0;
	int yNum = 0;
	for (auto const &m : b->Minerals())
	{
		xTotal += m->Pos().x;
		xNum++;
		yTotal += m->Pos().y;
		yNum++;
	}
	double xMean = (double)xTotal / (double)xNum;
	double yMean = (double)yTotal / (double)yNum;

	double numerator = 0;
	double denominator = 0;

	for (auto const &m : b->Minerals())
	{
		numerator += (m->Pos().x - xMean)*(m->Pos().y - yMean);
		denominator += (m->Pos().x - xMean)*(m->Pos().x - xMean);
	}

	double gradient = numerator / denominator;
	double yIntercept = yMean - gradient * xMean;

	int highestXDistance = INT_MIN;
	int lowestXDistance = INT_MAX;

	std::vector<double> gradientVector = { 1, gradient };
	for (auto const &m : b->Minerals())
	{
		int currentXDistance = m->Pos().x;
		if (highestXDistance != highestXDistance || highestXDistance < currentXDistance)
		{
			highestXDistance = currentXDistance;
		}
		if (lowestXDistance != lowestXDistance || lowestXDistance > currentXDistance)
		{
			lowestXDistance = currentXDistance;
		}
	}

	LineSegment returnValue;
	returnValue.end1 = Position( lowestXDistance - 32, (int)(gradient * lowestXDistance + yIntercept) );
	returnValue.end2 = Position(highestXDistance + 32, (int)(gradient * highestXDistance + yIntercept));

	mineralLines[b] = returnValue;
}

void calculateMineralLines()
{
	for (auto const &a : BWEM::Map::Instance().Areas())
	{
		for (const auto &b : a.Bases())
		{
			calculateMineralLine(&b);
		}
	}
}

void initializeMapAnalyser()
{
	isInit = true;
	calculateMineralLines();
	Broodwar->self()->getStartLocation();
}

LineSegment mineralLine(const BWEM::Base * b)
{
	assert(isInit);
	return mineralLines[b];
}

// pathToTilePosition
// Uses the A* algorithm, based on the described pseudocode in Wikipedia
//https://en.wikipedia.org/wiki/A*
//
// Does the calculations starting from end and travelling to begin, so that when backtracking we can make the vector the right way around without sacrificing speed.

TilePosition getMinTile(std::unordered_set<TilePosition> evaluableTiles, std::unordered_map<TilePosition, int> fScore);
int manhattenDistance(TilePosition p1, TilePosition p2);

#define MAX_ITERATIONS 200

std::vector<TilePosition> pathToTilePosition(TilePosition begin, TilePosition end)
{
	std::unordered_set<TilePosition> openSet;
	std::unordered_set<TilePosition> openSetErase;
	std::unordered_set<TilePosition> openSetAdd;
	std::unordered_set<TilePosition> closedSet;
	std::unordered_map<TilePosition, int> gScore;
	std::unordered_map<TilePosition, int> fScore;
	std::unordered_map<TilePosition, TilePosition> cameFrom;
	std::vector<TilePosition> returnVector;

	int iterations = 0;

	openSet.insert(begin);
	gScore[begin] = 0;
	fScore[begin] = manhattenDistance(begin, end);

	while (!openSet.empty())
	{
		iterations++;
		if (iterations > MAX_ITERATIONS)
		{
			Broodwar << "MAX_ITERATIONS Reached in pathToTilePosition" << std::endl;
			return returnVector;
		}

		for (auto const &t : openSetErase){ openSet.erase(t); }
		openSetErase.clear();
		for (auto const &t : openSetAdd){ openSet.insert(t); }
		openSetAdd.clear();
		for (auto const &t : openSet)
		{
			TilePosition currentTile = getMinTile(openSet, fScore);
			if (currentTile == end)
			{
				returnVector.insert(returnVector.end(), end);
				while (cameFrom.count(currentTile) == 1)
				{
					currentTile = cameFrom[currentTile];
					returnVector.insert(returnVector.end(), currentTile);
				}
				return returnVector;
			}
			openSetErase.insert(currentTile);
			closedSet.insert(currentTile);

			TilePosition neighbours[4] = { TilePosition(1, 0), TilePosition(-1, 0), TilePosition(0, 1), TilePosition(0, -1) };
			for (auto const &n : neighbours)
			{
				TilePosition neighbour = currentTile + n;
				if (closedSet.count(neighbour) == 1)
				{
					continue;
				}
				else if (neighbour.isValid() && theMap.GetTile(neighbour).Walkable())
				{
					openSetAdd.insert(neighbour);
				}

				int tentative_gScore = gScore[currentTile] + 1;
				if (tentative_gScore >= gScore[neighbour] && gScore[neighbour] != 0)
				{
					continue;
				}
				else
				{
					cameFrom[neighbour] = currentTile;
					gScore[neighbour] = tentative_gScore;
					fScore[neighbour] = gScore[neighbour] + manhattenDistance(neighbour, end);
				}
			}

		}
	}

	return returnVector;
}

TilePosition getMinTile(std::unordered_set<TilePosition> evaluableTiles, std::unordered_map<TilePosition, int> fScore)
{
	int bestScore = 10000;
	TilePosition bestTile;
	for (auto const &t : evaluableTiles)
	{
		if (fScore[t] < bestScore)
		{
			bestScore = fScore[t];
			bestTile = t;
		}
	}
	return bestTile;
}

int manhattenDistance(TilePosition p1, TilePosition p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}



TilePosition areaCenter(const BWEM::Area * area)
{
	return (area->TopLeft() + area->BottomRight()) / 2;
}

Position averageWeightedPosition(std::unordered_map<Position,double> weights)
{
	
	double outputX = 0;
	double outputY = 0;
	double totalWeight = 0;

	for (auto const &w : weights)
	{
		assert(w.second != 0);
		outputX += w.first.x * w.second;
		outputY += w.first.y * w.second;
		totalWeight += w.second;
	}

	outputX /= totalWeight;
	outputY /= totalWeight;

	return Position((int)outputX, (int)outputY);
}