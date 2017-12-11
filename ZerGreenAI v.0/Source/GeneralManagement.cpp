#include "stdafx.h"

#include "GeneralManagement.hpp"
#include "Timer.hpp"
#include "Namespaces.hpp"
#include "Hashes.hpp"

std::unordered_set<Manager *> Manager::managers;
std::unordered_set<Manager *> Manager::cleanUpList;

std::unordered_set<Manager*> &ZerGreenAI::Manager::ensureManagersCleanedUp()
{
	for (Manager * m : cleanUpList)
	{
		managers.erase(m);
	}
	cleanUpList.clear();
	return managers;
}

Manager::Manager()
{
	managers.insert(this);
}

Manager::~Manager()
{
	cleanUpList.insert(this);
}

void Manager::globalOnStart()
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		onStartTimerStart(m->name());
		m->onStart();
		onStartTimerEnd(m->name());
	}
}

void Manager::globalOnEnd(bool isWinner)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onEnd(isWinner);
	}
}

void Manager::globalOnFrame()
{
	for (Manager * m : ensureManagersCleanedUp())
	{
		if (cleanUpList.count(m) == 0)
		{
			startTimer(m->name());

			m->onFrame();

			if (cleanUpList.count(m) == 0)
				endTimer(m->name());
		}
	}
}

void Manager::globalOnSendText(std::string text)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onSendText(text);
	}
}

void Manager::globalOnReceiveText(Player player, std::string text)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onReceiveText(player, text);
	}
}

void Manager::globalOnPlayerLeft(Player player)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onPlayerLeft(player);
	}
}

void Manager::globalOnNukeDetect(Position target)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onNukeDetect(target);
	}
}

void Manager::globalOnUnitDiscover(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitDiscover(unit);
	}
}

void Manager::globalOnUnitEvade(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitEvade(unit);
	}
}

void Manager::globalOnUnitShow(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitShow(unit);
	}
}

void Manager::globalOnUnitHide(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitHide(unit);
	}
}

void Manager::globalOnUnitCreate(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitCreate(unit);
	}
}

void Manager::globalOnUnitDestroy(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitDestroy(unit);
	}
}

void Manager::globalOnUnitMorph(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitMorph(unit);
	}
}

void Manager::globalOnUnitRenegade(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitRenegade(unit);
	}
}

void Manager::globalOnSaveGame(std::string gameName)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onSaveGame(gameName);
	}
}

void Manager::globalOnUnitComplete(Unit unit)
{
	for (auto const &m : ensureManagersCleanedUp())
	{
		m->onUnitComplete(unit);
	}
}