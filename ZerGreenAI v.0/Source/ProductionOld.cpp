#include "stdafx.h"

#include "ProductionOld.hpp"
#include "Construction.hpp"
#include "Debug.hpp"
#include "Namespaces.hpp"

inline void ProductionManagerOld::cycleQueue(UnitType prod)
{
	UnitType chosenType = ProduceLists[prod].front();
	ProduceLists[prod].push(chosenType);
	ProduceLists[prod].pop();
}

void ProductionManagerOld::attemptTrain(Unit u)
{
	assert(u != nullptr);
	if (ProduceLists[u->getType()].empty())
	{
		Broodwar << "Nothing in list to train" << std::endl;
		return;
	}

	UnitType chosenType = ProduceLists[u->getType()].front();

	if (u->canTrain(chosenType))
	{
		u->train(ProduceLists[u->getType()].front());
		cycleQueue(u->getType());
	}
	else
	{
		cycleQueue(u->getType());
	}
}

void ZerGreenAI::ProductionManagerOld::onStart()
{
	SetUnitWeight(UnitTypes::Protoss_Zealot, 1);
	SetUnitWeight(UnitTypes::Protoss_Scout, 1);
	SetUnitWeight(UnitTypes::Protoss_Reaver, 1);
	UpdateWeightLists();
}

void ProductionManagerOld::onFrame()
{
	UnitManager::onFrame();
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	for (auto const &u : assignedUnits)
	{
		if (!u->isCompleted())
		{
			continue;
		}

		int currentLine = 0;
		auto tempQueue = ProduceLists[u->getType()];
		while (!tempQueue.empty())
		{
			new debugText(CoordinateType::Map, u->getPosition() + Position(0, currentLine * 10), tempQueue.front().c_str(), Broodwar->getLatencyFrames());
			tempQueue.pop();
			currentLine++;
		}
		if (u->getTrainingQueue().size() > 0)
		{
			std::string name = u->getTrainingQueue().front().getName();
			name.insert(name.begin(), (char)BWAPI::Text::Green);
			new debugText(CoordinateType::Map, u->getPosition() + Position(0, -10), name.c_str(), Broodwar->getLatencyFrames());
		}

		if (u->getRemainingTrainTime() - 24 < 0)
		{
			if (u->getTrainingQueue().size() < 2)
			{
				attemptTrain(u);
			}

		}
		else if (u->getTrainingQueue().size() > 1)
		{
			// will need to fix things getting skipped in the queue if uncommented
			//u->cancelTrain();
		}
	}
}

void ProductionManagerOld::UpdateWeightLists()
{
	for (auto const &type : UnitTypes::allUnitTypes())
	{
		if (!type.canProduce())
		{
			continue;
		}

		std::deque<UnitType> initQueue;
		for (auto const produce : type.buildsWhat())
		{
			for (int i = 0; i < UnitWeights[produce]; i++)
			{
				initQueue.push_front(produce);
			}
		}
		std::random_shuffle(initQueue.begin(), initQueue.end());
		ProduceLists[type] = std::queue<UnitType>(initQueue);
	}
}

void ProductionManagerOld::SetUnitWeight(UnitType unit, int Weight)
{
	UnitWeights[unit] = Weight;
}

Unitset ProductionManagerOld::getProductionBuildings()
{
	return assignedUnits;
}