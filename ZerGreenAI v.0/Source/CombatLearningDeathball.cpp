#include "CombatLearningDeathball.h"

#include "rl.hpp"

namespace cld
{
	class Action
	{
		double value;
		UnitCommandType command;
		Position pos;
	public:
		Action(UnitCommandType parCommand);
	};

	Action::Action(UnitCommandType parCommand)
	{
		value = 0;
		pos = Positions::None;
		command = parCommand;
	}

	struct Input
	{
	
	};

	class Policy
	{

	};
}

using namespace cld;

typedef std::unordered_set<Action> ActionSet;

//std::unordered_map<UnitType, ActionSet> unitTypeActions;
ActionSet getUnitActions(Unit u)
{
	//if ()

	ActionSet returnSet;
	for (auto const &cmd : UnitCommandTypes::allUnitCommandTypes())
	{
		if (u->canIssueCommandType(cmd))
		{
			returnSet.insert(Action(cmd));
		}
	}
	return returnSet;
}

void updateActionWeights(cld::Input i, ActionSet &actionSet)
{

}

void onUnitTurn(Unit u)
{
	for (auto const &a : getUnitActions(u))
	{

	}
}