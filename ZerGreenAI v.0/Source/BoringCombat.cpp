#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include "EnemyMovement.hpp"
#include "IMPScoutManager.hpp"
#include "BoringCombat.hpp"
#include "Debug.hpp"

bool isGeyser(Unit u)
{
	return u->getType() == UnitTypes::Resource_Vespene_Geyser;
}

UnitFilter IsGeyser = isGeyser;

void ZerGreenAI::BoringCombatManager::onUnitTurn(BWAPI::Unit u)
{
	Unitset neutralUnits = u->getUnitsInRadius(200, IsNeutral && !IsFlying && !(IsMineralField || IsGeyser));
	if (neutralUnits.size() > 0)
	{
		new debugText(u->getPosition(), "Damn you and your rocks, Dustin Browder!", turnLength());
		u->attack(*neutralUnits.begin());
	}
	else
	{
		if (u->getOrder() == Orders::AttackUnit)
		{
			new debugText(u->getPosition(), "You're going down!", turnLength());
			return;
		}

		Position attackTarget;
		attackTarget = ZerGreenAIObj::mainInstance->enemyMovementManager->getNearestTrackedEnemyLocation(u->getPosition());
		if (!attackTarget.isValid())
		{
			new debugText(u->getPosition(), "Arrgh, where are you??", turnLength());
			attackTarget = (Position)ZerGreenAIObj::mainInstance->impScoutManager->getColdest();
		}
		else
		{
			new debugText(u->getPosition(), "You can run, but you can't hide", turnLength());
		}
		u->attack(attackTarget);
	}
}
