#include "stdafx.h"

#include <math.h>
#include <algorithm>

#include "ZerGreenAI.hpp"
#include "MapAnalyser.hpp"
#include "LayoutPlanner.hpp"
#include "bweml.hpp"
#include "Timer.hpp"
#include "Hashes.hpp"
#include "Debug.hpp"
#include "GlobalHarvesting.hpp"
#include "Namespaces.hpp"

LayoutNode mainNode;

std::unordered_set<TilePosition> pathPositions;

namespace std
{
	template<>
	struct hash<LockedPosition>
	{
		size_t operator()(LockedPosition lp) const
		{
			return lp.type.getID() + 101 * lp.pos.x + 25867 * lp.pos.y;
		}
	};

	size_t hash<LayoutNode>::operator()(LayoutNode node) const
	{
		return hash<TilePosition>()(node.node);
	}


	bool operator==(const LayoutNode &a, const LayoutNode &b)
	{
		return a.node == b.node && a.children == b.children;
	}

	bool operator==(const LockedPosition &a, const LockedPosition &b)
	{
		return (a.pos == b.pos) && (a.type == b.type);
	}
}

std::unordered_set<LockedPosition> lockedPositions;

// Follows simple rules:
//
// All tiles around must remain connected.
// The placed building must have at least one empty adjacent tile
// All buildings adjacent to placed building must have at least one empty adjacent tile
//
// Otherwise, simply iterate through all possible positions to look for an acceptable one.

bool PositionsIntersect(LockedPosition a, LockedPosition b)
{

	if (a.pos.x + a.type.tileWidth() > b.pos.x && b.pos.x + b.type.tileWidth() > a.pos.x)
	{
		if (a.pos.y + a.type.tileHeight() > b.pos.y && b.pos.y + b.type.tileHeight() > a.pos.y)
		{
			return true;
		}
	}
	return false;
}

bool LayoutPlanner::isPositionAvailable(TilePosition pos, UnitType type)
{
	if (!Broodwar->canBuildHere(pos, type))
	{
		return false;
	}

	LockedPosition testLockedPosition;
	testLockedPosition.pos = pos;
	testLockedPosition.type = type;
	for (auto const &l : lockedPositions)
	{
		if (PositionsIntersect(testLockedPosition, l))
		{
			return false;
		}
	}

	return true;
}

void LayoutNode::draw()
{
	if (nodeType == undefined)
	{
		Broodwar << "Attempted to draw undefined nodeType" << std::endl;
	}
	else if (nodeType == main)
	{
		Broodwar->drawCircleMap(Position(node), 10, Colors::Red, true);

		for (auto const &c : children)
		{
			if (c.nodeType != mineralEdge)
			{
				continue;
			}

			Broodwar->drawLineMap(Position(this->node), Position(c.node), Colors::Blue);
			for (auto const &d : children)
			{
				if (d.nodeType != mineralEdge)
				{
					continue;
				}

				Broodwar->drawLineMap(Position(c.node), Position(d.node), Colors::Blue);
			}
		}

	}
	else if (nodeType == corner)
	{
		Broodwar->drawCircleMap(Position(node), 7, Colors::Orange, true);
	}
	else if (nodeType == mineralEdge)
	{
		Broodwar->drawCircleMap(Position(node), 5, Colors::Blue, true);
	}
	else
	{
		BWEM::BWAPI_ext::drawDiagonalCrossMap(Position(node), Position(node + TilePosition(1, 1)), Colors::Purple);
	}

	for (auto c : children)
	{
		c.draw();
	}
}

bool isProtected(TilePosition)
{
	return false;
}

TilePosition getAssimilatorPosition()
{
	return ZerGreenAIObj::mainInstance->globalHarvestManager->getAssimilatorPosition();
}

TilePosition getNexusPosition()
{
	return ZerGreenAIObj::mainInstance->globalHarvestManager->getBasePosition();
}

// APPROX_PYLON_RADIUS = 7.5 * 32
#define APPROX_PYLON_RADIUS 240

int positionValue(TilePosition pos, UnitType type)
{
	if (!pos.isValid())
	{
		return -2;
	}

	if (theMap.GetNearestArea(pos)->Id() != theMap.GetArea(Broodwar->self()->getStartLocation())->Id())
	{
		return -1;
	}

	if (!Broodwar->canBuildHere(pos, type))
	{
		return 0;
	}

	if (type == UnitTypes::Protoss_Pylon)
	{
		return std::max(1, 10 - (int)Broodwar->getUnitsInRadius(Position(pos), APPROX_PYLON_RADIUS, IsBuilding).size());
	}

	return 1;
}

#define TEMPERATURE_INITIAL 10
#define TEMPERATURE_DROP 0.5

//https://en.wikipedia.org/wiki/Simulated_annealing

TilePosition randomNeighbour(double temperature, TilePosition startingPos)
{
	int deltaX = (rand() % (2 * (int)ceil(temperature))) - (int)ceil(temperature);
	int deltaY = (rand() % (2 * (int)ceil(temperature))) - (int)ceil(temperature);
	return startingPos + TilePosition(deltaX, deltaY);
}

TilePosition LayoutPlanner::getAvailablePosition(UnitType type)
{
	if (type == UnitTypes::Protoss_Assimilator)
	{
		return getAssimilatorPosition();
	}

	if (type == UnitTypes::Protoss_Nexus)
	{
		return getNexusPosition();
	}

	TilePosition bestPosition = Broodwar->self()->getStartLocation();
	int bestValue = 0;

	double temperature = TEMPERATURE_INITIAL;
	while (temperature > 0 )
	{
		TilePosition currPosition = randomNeighbour(temperature, bestPosition);
		if (positionValue(currPosition, type) >= bestValue)
		{
			bestPosition = currPosition;
			bestValue = positionValue(currPosition, type);
		}
		temperature -= TEMPERATURE_DROP;
	}

	if (!Broodwar->canBuildHere(bestPosition, type))
	{
		std::string outputText = type.getName();
		return TilePositions::None;
	}

	return bestPosition;
}

void LayoutPlanner::recycleUnit(Unit u)
{

}

void updateLayout(UnitType type)
{
}

void lockLayout(UnitType type, TilePosition pos) // Called when a building begins construction
{
	LockedPosition chosenPosition;
	chosenPosition.pos = pos;
	chosenPosition.type = type;
	lockedPositions.insert(chosenPosition);

}

void unlockLayout(UnitType type, TilePosition pos) // Called when a building is destroyed
{
	LockedPosition chosenPosition;
	chosenPosition.pos = pos;
	chosenPosition.type = type;
	lockedPositions.erase(chosenPosition);
}

std::unordered_set<LayoutNode> getNodesAroundBuilding(TilePosition buildPosition, UnitType buildType)
{
	std::unordered_set<LayoutNode> returnValue;
	returnValue.insert(LayoutNode(buildPosition + TilePosition(buildType.tileWidth(), 0), LayoutNode::LayoutNodeType::corner));
	returnValue.insert(LayoutNode(buildPosition + TilePosition(0, buildType.tileHeight()), LayoutNode::LayoutNodeType::corner));
	returnValue.insert(LayoutNode(buildPosition + TilePosition(buildType.tileWidth(), buildType.tileHeight()), LayoutNode::LayoutNodeType::corner));
	return returnValue;
}

void ZerGreenAI::drawHighLevelLayout()
{
	//mainNode.draw();

	//for (auto const & t : pathPositions)
	{
		//BWEM::BWAPI_ext::drawDiagonalCrossMap(Position(t), Position(t + TilePosition(1, 1)), Colors::Green);
	}


}

int pathDelta(int begin, int end)
{
	if (end > begin)
	{
		return 1;
	}
	else if (end < begin)
	{
		return -1;
	}
	return 1;
}

const BWEM::ChokePoint * getMainChokePoint(const BWEM::Area * area)
{
	return area->ChokePoints().front();
}

void LayoutPlanner::onStart()
{
	mainNode.node = Broodwar->self()->getStartLocation();
	mainNode.nodeType = LayoutNode::LayoutNodeType::main;
	mainNode.children = getNodesAroundBuilding(mainNode.node, UnitTypes::Protoss_Nexus);

	const BWEM::Area *mainArea = theMap.GetArea(mainNode.node);
	const BWEM::Base * mainBase = &theMap.GetArea(mainNode.node)->Bases().front();

	for (auto const &p : BWEM::BWAPI_ext::outerBorder(mainNode.node, UnitTypes::Protoss_Nexus.tileSize(),false))
	{
		pathPositions.insert(p);
	}


	// for (auto const &p : pathToTilePosition(mainNode.node, TilePosition(getMainChokePoint(mainArea)->Center())))
	//{
	//	pathPositions.insert(p);
	//}

	/*for (auto const &b : Broodwar->getStartLocations())
	{
		TilePosition pos1 = TilePosition(getMainChokePoint(mainArea)->Center());
		TilePosition pos2;
		for (auto const &segment : getMainChokePoint(mainArea)->GetPathTo(getMainChokePoint(theMap.GetArea(b))))
		{
			pos2 = TilePosition(segment->Center());
			for (auto const &p : pathToTilePosition(pos1, pos2))
			{
				pathPositions.insert(p);
			}
			pos1 = pos2;

		}
	}*/

	mainNode.children.insert(LayoutNode(TilePosition(mineralLine(mainBase).end1), LayoutNode::LayoutNodeType::mineralEdge));
	mainNode.children.insert(LayoutNode(TilePosition(mineralLine(mainBase).end2), LayoutNode::LayoutNodeType::mineralEdge));

}