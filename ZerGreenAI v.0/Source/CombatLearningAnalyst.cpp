#include "CombatLearningAnalyst.h"
#include "Hashes.h"
#include <fstream>
#include <vector>
using namespace combatLearning;

// private
class BattleRepresentation;

std::unordered_map<BattleRepresentation*, bool> data;
// end private

class BattleRepresentation
{
	std::unordered_map<UnitType, int> attackerTypes;
	std::unordered_map<UnitType, int> defenderTypes;

public:
	BattleRepresentation(Unitset attackers, Unitset defenders);
	double distanceTo(BattleRepresentation rep);
	BattleRepresentation(std::string s);
	std::string write(); // write should not write spaces
};

BattleRepresentation::BattleRepresentation(Unitset attackers, Unitset defenders)
{
	for (auto const &u : attackers)
	{
		attackerTypes[u->getType()]++;
	}
	for (auto const &u : defenders)
	{
		defenderTypes[u->getType()]++;
	}
}

#define NONE_OF_TYPE_DISTANCE_INCREMENTER 10
double unitTypeMultisetDistance(std::unordered_map<UnitType, int> a, std::unordered_map<UnitType, int> b)
{
	double distance = 0;
	for (auto const &t : a)
	{
		if (b[t.first] == 0)
		{
			distance += NONE_OF_TYPE_DISTANCE_INCREMENTER;
		}
		else
		{
			distance += (abs(t.second - b[t.first]));
		}
	}
	for (auto const &t : b)
	{
		if (a[t.first] == 0)
		{
			distance += NONE_OF_TYPE_DISTANCE_INCREMENTER;
		}
	}
	return distance;
}

double BattleRepresentation::distanceTo(BattleRepresentation rep)
{
	double distance = 0;
	distance += unitTypeMultisetDistance(attackerTypes, rep.attackerTypes);
	distance += unitTypeMultisetDistance(defenderTypes, rep.defenderTypes);
	return distance;
}

BattleRepresentation::BattleRepresentation(std::string s)
{

}

std::string writeMap(std::unordered_map<UnitType, int> map)
{
	std::string output = "";
	for (auto const &m : map)
	{
		output.append('T', 1);
		output.append(std::to_string((int)m.first));
		output.append('N', 1);
		output.append(std::to_string(m.second));
	}
	return output;
}

std::string BattleRepresentation::write()
{
	std::string output = "";
	output.append('A',1);
	output.append(writeMap(attackerTypes));
	output.append('D',1);
	output.append(writeMap(defenderTypes));
	return output;

}

void onStart()
{

}

void addData(Unitset attackers, Unitset defenders, bool didWin)
{

}

void appendData()
{

}

void saveData()
{

}

void loadData()
{

}

bool guessIfWin(Unitset attackers, Unitset defenders);