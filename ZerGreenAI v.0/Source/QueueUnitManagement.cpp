#include "QueueUnitManagement.h"

void QueueUnitManager::onAssignment(Unit u)
{
	unitDecideQueue.push(u);
}

void QueueUnitManager::onFrame()
{
	onUnitTurn(rotateQueue());
}

Unit QueueUnitManager::rotateQueue()
{
	Unit returnValue = unitDecideQueue.front();
	unitDecideQueue.pop();
	if (returnValue->exists())
	{
		unitDecideQueue.push(returnValue);
	}
	return returnValue;
}