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

int ZerGreenAI::QueueUnitManager::turnLength()
{
	return assignedUnits.size();
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

	Unit returnValue = nullptr;
	while ((returnValue == nullptr || !returnValue->exists()) && !unitDecideQueue.empty())
	{
		returnValue = unitDecideQueue.front();
		unitDecideQueue.pop_front();
	}

	unitDecideQueue.push_back(returnValue);
	return returnValue;

}