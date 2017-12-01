#include "QueueUnitManagement.hpp"
#include "Namespaces.hpp"

void QueueUnitManager::onAssignment(Unit u)
{
	unitDecideQueue.push_back(u);
}

void QueueUnitManager::recycleUnit(Unit u)
{
	std::remove(unitDecideQueue.begin(), unitDecideQueue.end(), u);
}

void QueueUnitManager::onFrame()
{
	if (!unitDecideQueue.empty())
		onUnitTurn(rotateQueue());
}

Unit QueueUnitManager::rotateQueue()
{
	Unit returnValue;
	do
	{
		returnValue = unitDecideQueue.front();
		unitDecideQueue.pop_front();
	} while (!returnValue->exists() && !unitDecideQueue.empty());

	if (unitDecideQueue.empty())
	{
		return nullptr;
	}
	else
	{
		unitDecideQueue.push_back(returnValue);
		return returnValue;
	}
}