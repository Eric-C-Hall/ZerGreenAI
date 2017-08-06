#include "GeneralManagement.h"

std::unordered_set<Manager *> Manager::managers;

Manager::Manager()
{
	managers.insert(this);
}

void Manager::globalOnStart()
{
	for (auto const &m : managers)
	{
		m->onStart();
	}
}

void Manager::globalOnEnd(bool isWinner)
{
	for (auto const &m : managers)
	{
		m->onEnd(isWinner);
	}
}

void Manager::globalOnFrame()
{
	for (auto const &m : managers)
	{
		m->onFrame();
	}
}

void Manager::globalOnSendText(std::string text)
{
	for (auto const &m : managers)
	{
		m->onSendText(text);
	}
}

void Manager::globalOnReceiveText(Player player, std::string text)
{
	for (auto const &m : managers)
	{
		m->onReceiveText(player, text);
	}
}

void Manager::globalOnPlayerLeft(Player player)
{
	for (auto const &m : managers)
	{
		m->onPlayerLeft(player);
	}
}

void Manager::globalOnNukeDetect(Position target)
{
	for (auto const &m : managers)
	{
		m->onNukeDetect(target);
	}
}

void Manager::globalOnUnitDiscover(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitDiscover(unit);
	}
}

void Manager::globalOnUnitEvade(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitEvade(unit);
	}
}

void Manager::globalOnUnitShow(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitShow(unit);
	}
}

void Manager::globalOnUnitHide(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitHide(unit);
	}
}

void Manager::globalOnUnitCreate(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitCreate(unit);
	}
}

void Manager::globalOnUnitDestroy(Unit unit)
{
	for (auto const &m : managers)
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
	for (auto const &m : managers)
	{
		m->onUnitRenegade(unit);
	}
}

void Manager::globalOnSaveGame(std::string gameName)
{
	for (auto const &m : managers)
	{
		m->onSaveGame(gameName);
	}
}

void Manager::globalOnUnitComplete(Unit unit)
{
	for (auto const &m : managers)
	{
		m->onUnitComplete(unit);
	}
}