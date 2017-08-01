#pragma once

#include "Namespaces.h"

struct SimulationResults
{
	bool didWin;
	int mineralLoss;
	int gasLoss;
	int mineralDamage;
	int gasDamage;

	void setDefault();
};

struct SimulatedUnit
{
	int id;
	int hp;
	int cooldownFrames;
	Player owner;
	UnitType type;
};

SimulationResults simulateAttack(Unitset attackers, Unitset defenders);