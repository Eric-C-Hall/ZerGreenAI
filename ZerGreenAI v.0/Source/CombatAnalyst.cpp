#include "stdafx.h"

#include "CombatAnalyst.hpp"
#include "Namespaces.hpp"

namespace std
{
	template<>
	struct hash<SimulatedUnit>
	{
		size_t operator()(SimulatedUnit sim)
		{
			return sim.id;
		}
	};

	bool operator==(SimulatedUnit a, SimulatedUnit b)
	{
		return a.id == b.id;
	}
}

typedef std::vector<SimulatedUnit> SimUnitvector;

template <class UnaryPredicate>
void ZerGreenAI::eraseRemoveIdiom(SimUnitvector &vector, UnaryPredicate pred)
{
	auto firstValid = std::remove_if(vector.begin(), vector.end(), pred);
	if (firstValid == vector.end())
	{
		vector.clear();
	}
	else
	{
		vector.erase(firstValid);
	}
}

void SimulationResults::setDefault()
{
	didWin = false;
	mineralDamage = 0;
	mineralLoss = 0;
	gasDamage = 0;
	gasLoss = 0;
}

SimulatedUnit unit2Sim(Unit u)
{
	SimulatedUnit returnValue;
	returnValue.type = u->getType();
	returnValue.hp = u->getHitPoints() + u->getShields();
	returnValue.cooldownFrames = 0;
	returnValue.owner = u->getPlayer();
	returnValue.id = u->getID();
	return returnValue;
}

void simulateAttackFrame(SimUnitvector &firing, SimUnitvector &receiving)
{
	for (auto &u : firing)
	{
		if (u.cooldownFrames > 0)
		{
			u.cooldownFrames--;
			continue;
		}

		SimulatedUnit *victim = &receiving[rand() % receiving.size()];
		int damage = Broodwar->getDamageTo(victim->type, u.type, victim->owner, u.owner);
		victim->hp -= damage;

		if (victim->type.isFlyer())
		{
			u.cooldownFrames = u.type.airWeapon().damageCooldown();
		}
		else
		{
			u.cooldownFrames = u.type.groundWeapon().damageCooldown();
		}
	}
}

bool isSimUnitAlive(SimulatedUnit sim)
{
	return sim.hp > 0;
}

void cleanUpDeadUnits(SimUnitvector &vector, int &lostMinerals, int &lostGas)
{
	for (auto const &u : vector)
	{
		if (u.hp <= 0)
		{
			lostMinerals += u.type.mineralPrice();
			lostGas += u.type.gasPrice();
		}
	}
	eraseRemoveIdiom(vector, isSimUnitAlive);
}

#define MAX_COMBAT_SIM_FRAMES 1000

bool checkIfWon(SimulationResults &results, const SimUnitvector &simAttackers, const SimUnitvector &simDefenders)
{
	if (simAttackers.empty())
	{
		results.didWin = false;
		return true;
	}
	else if (simDefenders.empty())
	{
		results.didWin = true;
		return true;
	}
	return false;
}

bool isNotGarbage(SimulatedUnit sim)
{
	return !(sim.type == UnitTypes::Unknown || !sim.type.canAttack());
}

void removeGarbage(SimUnitvector &force)
{
	eraseRemoveIdiom(force, isNotGarbage);
}

SimulationResults ZerGreenAI::simulateAttack(Unitset attackers, Unitset defenders)
{
	SimUnitvector simAttackers;
	SimUnitvector simDefenders;
	SimulationResults results;
	results.setDefault();

	int lostMinerals;
	int lostGas;
	int damageMinerals;
	int damageGas;

	if (attackers.empty())
	{
		results.didWin = false;
		return results;
	}
	else if (defenders.empty())
	{
		results.didWin = true;
		return results;
	}

	for (auto const &u : attackers)
	{
		simAttackers.insert(simAttackers.end(),unit2Sim(u));
	}
	for (auto const &u : defenders)
	{
		simDefenders.insert(simDefenders.end(),unit2Sim(u));
	}

	removeGarbage(simAttackers);
	removeGarbage(simDefenders);

	if (checkIfWon(results, simAttackers, simDefenders))
	{
		return results;
	}

	int numSims = 0;
	while (!simAttackers.empty() && !simDefenders.empty())
	{
		simulateAttackFrame(simDefenders, simAttackers);
		simulateAttackFrame(simAttackers, simDefenders);

		cleanUpDeadUnits(simAttackers, lostMinerals, lostGas);
		cleanUpDeadUnits(simDefenders, damageMinerals, damageGas);

		if (numSims > MAX_COMBAT_SIM_FRAMES)
		{
			Broodwar << "Error: num comSimFrames exceeds " << MAX_COMBAT_SIM_FRAMES << std::endl;
			return results;
		}
		numSims++;
	}
	
	checkIfWon(results, simAttackers, simDefenders);
	return results;
}