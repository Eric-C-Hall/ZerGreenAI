#include "CombatLearningAnalyst.h"
#include "Hashes.h"
#include <fstream>
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
	std::stringstream stream;
	stream.str(s);
	char currentChar = '\0';
	int convertEnum;
	bool isAttacker = true;

	while (currentChar != 'X')
	{
		stream >> currentChar;
		stream >> convertEnum;

		switch (currentChar)
		{
		case 'A':
			isAttacker = true;
			break;
		case 'D':
			isAttacker = false;
			break;
		case 'T':
			UnitType tempType;
			tempType = (UnitType)convertEnum;
			stream >> currentChar;
			stream >> convertEnum;
			if (isAttacker)
			{
				attackerTypes[tempType] = convertEnum;
			}
			else
			{
				defenderTypes[tempType] = convertEnum;
			}
			break;
		}
	}
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
	output.append('0', 1);
	output.append(writeMap(attackerTypes));
	output.append('D',1);
	output.append('0', 1);
	output.append(writeMap(defenderTypes));
	output.append('X', 1);
	output.append('0', 1);
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
	std::ofstream saveFile;
	saveFile.open("bwapi-data/write/CombatLearning.txt");

	if (!saveFile.is_open()) {
		Broodwar << "File could not be opened" << std::endl;
		return;
	}

	for (auto const &r : data)
	{
		saveFile << 'R' << r.first->write() << ' ' << r.second;
	}
	saveFile << 'X' << "X 0";
}

void loadData()
{
	std::ifstream loadFile;
	loadFile.open("bwapi-data/write/CombatLearning.txt");

	if (!loadFile.is_open()) {
		Broodwar << "File could not be opened" << std::endl;
		return;
	}

	char isEnd = '\0';
	std::string rep;
	bool didWin;

	while (!loadFile.bad() && isEnd != 'X')
	{
		loadFile >> isEnd;
		loadFile >> rep;
		loadFile >> didWin;

		switch (isEnd)
		{
		case 'R':
			data[new BattleRepresentation(rep)] = didWin;
			break;
		case 'X':
			break;
		}
	}
}

bool guessIfWin(Unitset attackers, Unitset defenders);