#include "stdafx.h"

#include "QueueUnitManagement.hpp"
#include "Namespaces.hpp"

void QueueUnitManager::onAssignment(Unit u)
{
	UnitManager::onAssignment(u);
	unitDecideQueue.push_back(u);
}

void QueueUnitManager::onReassignment(Unit u)
{
	UnitManager::onReassignment(u);
	std::remove(unitDecideQueue.begin(), unitDecideQueue.end(), u);
}

void QueueUnitManager::onFrame()
{
	UnitManager::onFrame();
	if (!unitDecideQueue.empty())
	{
		onUnitTurn(rotateQueue());
	}
}

Unit QueueUnitManager::rotateQueue()
{
	if (unitDecideQueue.empty())
	{
		return nullptr;
	}

	Unit returnValue;
	do
	{
		returnValue = unitDecideQueue.front();
		unitDecideQueue.pop_front();
	} while (!returnValue->exists() && !unitDecideQueue.empty());

	unitDecideQueue.push_back(returnValue);

	Broodwar << "rotate Queue" << returnValue->getType().c_str() << std::endl;
	return returnValue;

}