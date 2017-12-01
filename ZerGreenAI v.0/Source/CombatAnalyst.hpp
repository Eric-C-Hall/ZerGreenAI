#pragma once

#include <BWAPI.h>

namespace ZerGreenAI
{

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
		BWAPI::Player owner;
		BWAPI::UnitType type;
	};

	SimulationResults simulateAttack(BWAPI::Unitset attackers, BWAPI::Unitset defenders);

}