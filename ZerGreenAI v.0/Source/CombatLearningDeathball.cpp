#include "CombatLearningDeathball.h"

#include "rl.hpp"
#include "Hashes.h"

namespace cld
{
	class Action;
}
namespace std
{
	bool operator==(cld::Action a, cld::Action b);
}

namespace cld
{
	class Action
	{
		UnitCommandType command;
		Position relativePos;
		int extra;
	public:
		double value;
		Action(UnitCommandType parCommand, Position pos = Positions::None, int parExtra = 0);
		void executeAction(Unit u);
		friend std::hash<Action>;
		friend bool std::operator==(Action a, Action b);
	};

	Action::Action(UnitCommandType parCommand, Position parRelativePos, int parExtra)
	{
		value = 0;
		relativePos = parRelativePos;
		command = parCommand;
		extra = parExtra;
	}

	struct Input
	{
		enum InputType
		{
			unit
		};
		InputType inputType;
		Unit whichUnit;
	};
}

namespace std
{
	template<>
	struct hash < cld::Action >
	{
		size_t operator()(cld::Action a) const
		{
			return hash<Position>()(a.relativePos) * 251 + a.command;
		}
	};

	bool operator==(cld::Action a, cld::Action b)
	{
		return a.command = b.command && a.relativePos == b.relativePos;
	}
}

using namespace cld;

typedef std::unordered_set<Action> ActionSet;
typedef std::unordered_set<Input *> InputSet;

namespace cld
{
	class Policy
	{
	public:
		void updateActionWeights(Input * i, ActionSet &actionSet)
		{

		}
	};
}

std::unordered_map<UnitType, ActionSet> unitTypeActions;
std::unordered_map<Unit, Policy *> unit2Policy;
std::unordered_map<UnitType, std::queue<Policy *>> unusedPolicies;
std::unordered_set<Policy *> testedPolicyPool;

#define NUM_TILES_ACTION_RANGE 5
ActionSet getUnitActions(Unit u)
{
	ActionSet relevantActions;
	for (int x = -NUM_TILES_ACTION_RANGE; x < NUM_TILES_ACTION_RANGE; x++)
	{
		for (int y = -NUM_TILES_ACTION_RANGE; y < NUM_TILES_ACTION_RANGE; y++)
		{
			relevantActions.insert(Action(UnitCommandTypes::Attack_Move, Position(x * TILEPOSITION_SCALE, y * TILEPOSITION_SCALE)));
			relevantActions.insert(Action(UnitCommandTypes::Move, Position(x * TILEPOSITION_SCALE, y * TILEPOSITION_SCALE)));
		}
	}
	relevantActions.insert(Action(UnitCommandTypes::Stop));
	relevantActions.insert(Action(UnitCommandTypes::None));


	/*ActionSet &relevantActions = unitTypeActions[u->getType()];
	if (relevantActions.empty())
	{
		ActionSet buildActions;
		for (auto const &cmd : UnitCommandTypes::allUnitCommandTypes())
		{
			if (u->canIssueCommandType(cmd))
			{
				buildActions.insert(Action(cmd));
			}
		}
		unitTypeActions[u->getType()] = buildActions;
		return buildActions;
	}
	else
	{
		return relevantActions;
	}*/
}

InputSet getRelevantInput(Unit myUnit)
{

}

double updateUnitPolicyValue(Unit u)
{

}

void processPoliciesInPool()
{

}

Policy * newRandomPolicy()
{

}

void LearningDeathballManager::onAssignmentQ(Unit u)
{
	auto pQueue = unusedPolicies[u->getType()];
	Policy * policy = pQueue.front();

	if ( policy == nullptr)
	{
		processPoliciesInPool();
		policy = unusedPolicies[u->getType()].front();
		if (policy == nullptr)
		{
			policy = newRandomPolicy();
		}
		else
		{
			pQueue.pop();
		}
	}
	else
	{
		pQueue.pop();
	}

	unit2Policy[u] = policy;	
}

void LearningDeathballManager::recycleUnit(Unit u)
{
	updateUnitPolicyValue(u);
	testedPolicyPool.insert(unit2Policy[u]);
	unit2Policy.erase(u);
}

void Action::executeAction(Unit u)
{
	Position pos = u->getPosition() + relativePos;
	u->issueCommand(UnitCommand(u, command, nullptr, pos.x, pos.y, extra));
}

void LearningDeathballManager::onUnitTurn(Unit u)
{
	ActionSet unitActions = getUnitActions(u);
	for (auto const &i : getRelevantInput(u))
	{
		unit2Policy[u]->updateActionWeights(i, unitActions);
		delete i;
	}
	Action bestAction = Action(UnitCommandTypes::None);
	double bestValue = 0;
	for (auto const &a : unitActions)
	{
		if (a.value > bestValue)
		{
			bestAction = a;
			bestValue = a.value;
		}
	}
	bestAction.executeAction(u);
}