#include "Deathball.h"
#include "ScoutAnalysis.h"
#include "Debug.h"
#include "MapAnalyser.h"
#include "Hashes.h"
#include "CombatAnalyst.h"

#define TOGETHER_DISTANCE 150
#define TOGETHER_FRACTION 3

#define NUM_SKIP_FRAME 15
#define TEXT_VISIBLE_LENGTH 15
#define NUM_UNITS_AT_ONCE 10

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

Position getStormLocation(Unit u)
{
	for (auto const &e : u->getUnitsInRadius(288, IsEnemy))
	{
		int numEnemyUnits = e->getUnitsInRadius(48, IsEnemy).size();
		int numAlliedUnits = e->getUnitsInRadius(48, IsAlly).size();

		new debugCircle(e->getPosition(), 48, Colors::Blue,120);

		if (numEnemyUnits - numAlliedUnits >= NUM_IN_STORM)
		{
			return e->getPosition();
		}
	}

	return Positions::None;
}

bool doSpecialStuff(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Reaver)
	{
		if (u->getTrainingQueue().size() > 0) return false;

		new debugText(u->getPosition(), "Baby's Coming", TEXT_VISIBLE_LENGTH);
		u->train(UnitTypes::Protoss_Scarab);
		return true;
	}
	else if (u->getType() == UnitTypes::Protoss_High_Templar)
	{
		Position stormLocation = getStormLocation(u);
		if (stormLocation == Positions::None) return false;

		new debugText(u->getPosition(), "Feel the Wrath of a Templar!", TEXT_VISIBLE_LENGTH);
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

	if (u->getDistance(currentBallPosition) > TOGETHER_DISTANCE)
	{
		new debugText(u->getPosition(), "Wait for me!", TEXT_VISIBLE_LENGTH);
		u->move(currentBallPosition);
	}
	else if (wouldWinConfrontation)
	{
		new debugText(u->getPosition(), "Let's Get 'Em!", TEXT_VISIBLE_LENGTH);
		u->attack(goal);
	}
	else
	{
		new debugText(u->getPosition(), "Damn Coward", TEXT_VISIBLE_LENGTH);
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
	new debugLine(u->getPosition(), finalPosition, Colors::Green, NUM_SKIP_FRAME);
	new debugCircle(finalPosition, 2, Colors::Green, NUM_SKIP_FRAME, true);
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
	unitDecideQueue.push(returnValue);
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

	auto path = theMap.GetPath(currentBallPosition, Position(areaCenter(highPriTarget)));
	
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
	if (Broodwar->getFrameCount() % NUM_SKIP_FRAME != 0)
	{
		return;
	}

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
	wouldWinConfrontation = simulateAttack(getNearbyUnits(), Broodwar->enemy()->getUnits()).didWin;
}

DeathballManager::DeathballManager()
{
	currentBallPosition = Position(Broodwar->self()->getStartLocation());
	goal = currentBallPosition;
	wouldWinConfrontation = false;
}