#include "stdafx.h"

#include "Deathball.hpp"
#include "ScoutAnalysis.hpp"
#include "Debug.hpp"
#include "MapAnalyser.hpp"
#include "Hashes.hpp"
#include "CombatAnalyst.hpp"
#include "Namespaces.hpp"

#define ENEMY_NEAR_DISTANCE 1000
#define TOGETHER_DISTANCE 200
#define TOGETHER_FRACTION 3

#define BASE_DEFENCE_DISTANCE 1000

#define TEXT_VISIBLE_LENGTH 15
#define NUM_UNITS_AT_ONCE 1

void DeathballManager::onAssignment(Unit u)
{
	unitDecideQueue.push(u);
}

void DeathballManager::recycleUnit(Unit u)
{
	
}

bool DeathballManager::isTogether()
{
	unsigned int numHere = 0;
	for (auto const &u : assignedUnits)
	{
		if (u->getDistance(currentBallPosition) < TOGETHER_DISTANCE)
		{
			numHere++;
		}
	}

	//new debugCircle(currentBallPosition, TOGETHER_DISTANCE, Colors::Purple, NUM_SKIP_FRAME);

	return numHere > assignedUnits.size() / TOGETHER_FRACTION;
}

#define COHESION_MODIFIER 3   // Increasing reduces cohesion
#define INDIVIDUAL_SEPERATION_MODIFIER 1 // increasing does nothing I think
#define SEPERATION_MODIFIER 10 // Increasing increases seperation
#define PROGRESS_MODIFIER 800 // Increasing increases motivation

#define SHOW_FINAL 1
#define SHOW_SEPERATION 0

#define NUM_IN_STORM 3
#define STORM_RADIUS 64

bool isStormTargetable(Unit u)
{
	return !IsBuilding(u);
}

UnitFilter IsStormTargetable = isStormTargetable;

Position getStormLocation(Unit u)
{
	for (auto const &e : u->getUnitsInRadius(288, IsEnemy))
	{
		int numEnemyUnits = e->getUnitsInRadius(STORM_RADIUS, IsEnemy && IsStormTargetable).size();
		int numAlliedUnits = e->getUnitsInRadius(STORM_RADIUS, IsAlly && IsStormTargetable).size();

		if (numEnemyUnits - numAlliedUnits >= NUM_IN_STORM)
		{
			return e->getPosition();
		}
	}

	return Positions::None;
}

#define TEMPLAR_LOW_ENERGY 50
bool isTemplarLowEnergy(Unit u)
{
	return u->getType() == UnitTypes::Protoss_High_Templar && u->getEnergy() < TEMPLAR_LOW_ENERGY;
}

bool doSpecialStuff(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Reaver)
	{
		if (u->getTrainingQueue().size() > 0 || u->getScarabCount() > ( Broodwar->self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) == 1 ?9:4)) return false;

		debugUnitText(u, "Baby's Coming");
		u->train(UnitTypes::Protoss_Scarab);
		return true;
	}
	else if (u->getType() == UnitTypes::Protoss_High_Templar)
	{
		if (isTemplarLowEnergy(u))
		{
			debugUnitText(u, "Looking for a buddy.");

			Unit mergeBuddy = u->getClosestUnit(isTemplarLowEnergy);
			if (mergeBuddy != nullptr)
			{
				debugUnitText(u, "Super Saiyan mode Engage!");
				u->useTech(TechTypes::Archon_Warp, mergeBuddy);
				return true;
			}
		}

		Position stormLocation = getStormLocation(u);
		if (stormLocation == Positions::None || !u->canUseTech(TechTypes::Psionic_Storm,stormLocation) || u->getEnergy() < TechTypes::Psionic_Storm.energyCost()) return false;

		new debugCircle(stormLocation, STORM_RADIUS, Colors::Blue, 120);
		debugUnitText(u, "Feel the Wrath of a Templar!");
		u->useTech(TechTypes::Psionic_Storm, stormLocation);
		return true;
	}

	return false;
}

void DeathballManager::goDirect(Unit u)
{
	if (doSpecialStuff(u))
	{
		return;
	}

	bool isNearBase = u->getDistance(Position(Broodwar->self()->getStartLocation())) < BASE_DEFENCE_DISTANCE;

	if (u->getDistance(currentBallPosition) > TOGETHER_DISTANCE)
	{
		debugUnitText(u, "Wait for me!");
		u->move(currentBallPosition);
	}
	else if (wouldWinConfrontation || isNearBase)
	{
		Unit enemy = u->getClosestUnit(IsEnemy && (CanAttack || IsTransport));
		Unit passiveEnemy = u->getClosestUnit(IsEnemy);
		if (enemy != nullptr)
		{
			if (isNearBase)
			{
				debugUnitText(u, "Defend in the name of the Khala!");
			}
			else
			{
				debugUnitText(u, "RAWR!!!!");
			}
			u->attack(enemy);
		}
		else if (passiveEnemy != nullptr)
		{
			u->attack(passiveEnemy);
			debugUnitText(u,"You shall be Cleansed.");
		}
		else
		{
			debugUnitText(u, "Let's Get 'Em!");
			u->attack(goal);
		}
	}
	else
	{
		debugUnitText(u, "Damn Coward");
		u->move(Position(Broodwar->self()->getStartLocation()));
	}
}

void DeathballManager::unitDecide(Unit u)
{
	std::unordered_map<Position, double> positionWeights;

	// Weight 1: Cohesion

	positionWeights[currentBallPosition] = (double) (u->getDistance(currentBallPosition) + 1) / (double) COHESION_MODIFIER;

	// Weight 2: Seperation
	
	std::unordered_map<Position, double> seperationWeights;

	for (auto const &n : assignedUnits)
	{
		if (n == u)
		{
			continue;
		}
		seperationWeights[u->getPosition() * 2 - n->getPosition()] = (double)INDIVIDUAL_SEPERATION_MODIFIER / ((double)u->getDistance(n) + 1);
	}

	if (!seperationWeights.empty())
	{
		Position averageSeperation = averageWeightedPosition(seperationWeights);
		positionWeights[averageSeperation] = SEPERATION_MODIFIER;
#if SHOW_SEPERATION && _DEBUG
		new debugLine(u->getPosition(), averageSeperation, Colors::Orange, NUM_SKIP_FRAME);
		new debugCircle(averageSeperation, 4, Colors::Orange, NUM_SKIP_FRAME, true);
#endif
	}
	
	// Weight 3: Progress

	positionWeights[goal] = (double) PROGRESS_MODIFIER / (double) (u->getDistance(goal) + 1);

	Position finalPosition = averageWeightedPosition(positionWeights);

#if SHOW_FINAL && _DEBUG
	new debugLine(u->getPosition(), finalPosition, Colors::Green, 100);
	new debugCircle(finalPosition, 2, Colors::Green, 100, true);
#endif

	if (true)
	{
		u->move(finalPosition);
	}
	
}

Unit DeathballManager::rotateQueue()
{
	Unit returnValue = unitDecideQueue.front();
	unitDecideQueue.pop();
	if (returnValue->exists())
	{
		unitDecideQueue.push(returnValue);
	}
	return returnValue;
}

#define CLOSE_ENOUGH_TO_GOAL 100

void DeathballManager::updateGoal()
{
	auto highPriTarget = getHighestPriorityTarget();
	if (highPriTarget == nullptr)
	{
		return;
	}

	BWEM::CPPath path;

	if (theMap.GetTile(TilePosition(currentBallPosition)).Walkable())
	{
		path = theMap.GetPath(currentBallPosition, Position(areaCenter(highPriTarget)));
	}
	else
	{
		path = theMap.GetPath(Position(areaCenter(theMap.GetNearestArea(TilePosition(currentBallPosition)))),Position(areaCenter(highPriTarget)));
	}
	
	if (path.size() > 1 && currentBallPosition.getApproxDistance(Position(path.front()->Center())) < CLOSE_ENOUGH_TO_GOAL)
	{
		goal = Position(path[1]->Center());
	}
	else if (!path.empty() && currentBallPosition.getApproxDistance(Position(path.front()->Center())) > CLOSE_ENOUGH_TO_GOAL)
	{
		goal = Position(path.front()->Center());
	}
	else
	{
		goal = Position(areaCenter(highPriTarget));
	}

}

Unitset DeathballManager::getNearbyEnemies()
{
	return Broodwar->getUnitsInRadius(currentBallPosition,ENEMY_NEAR_DISTANCE,IsEnemy);
}

Unitset DeathballManager::getNearbyUnits()
{
	Unitset nearbyUnits;
	for (auto const &u : assignedUnits)
	{
		if (u->getDistance(currentBallPosition) < TOGETHER_DISTANCE)
		{
			nearbyUnits.insert(u);
		}
	}
	return nearbyUnits;
}

void DeathballManager::updateBallPosition()
{
	Unitset nearbyUnits;
	nearbyUnits = getNearbyUnits();

	if (!nearbyUnits.empty())
	{
		currentBallPosition = nearbyUnits.getPosition();
	}
	else if (!assignedUnits.empty())
	{
		currentBallPosition = assignedUnits.getPosition();
	}
}

#define SHOW_BALL_POSITION 1
void DeathballManager::onFrame()
{
	updateBallPosition();
	updateGoal();
	updateConfrontation();
#if SHOW_BALL_POSITION
	Broodwar->drawCircleMap(goal, 6, Colors::Red, true);
	Broodwar->drawCircleMap(currentBallPosition, 6, Colors::Purple, true);
	Broodwar->drawCircleMap(currentBallPosition, TOGETHER_DISTANCE, Colors::Purple, false);
#endif

	if (!unitDecideQueue.empty())
	{
		for (int i = 0; i < NUM_UNITS_AT_ONCE; i++)
		{
			goDirect(rotateQueue());
		}
	}
	
}

void DeathballManager::updateConfrontation()
{
	wouldWinConfrontation = simulateAttack(getNearbyUnits(), getNearbyEnemies()).didWin;
}

DeathballManager::DeathballManager()
{
	currentBallPosition = Position(Broodwar->self()->getStartLocation());
	goal = currentBallPosition;
	wouldWinConfrontation = false;
}