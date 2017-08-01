#include "Production.h"
#include "Construction.h"
#include "Debug.h"

ProductionManager* LocalProductionManager;

inline void ProductionManager::cycleQueue(UnitType prod)
{
	UnitType chosenType = ProduceLists[prod].front();
	ProduceLists[prod].push(chosenType);
	ProduceLists[prod].pop();
}

void ProductionManager::attemptTrain(Unit u)
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

void ProductionManager::onFrame()
{
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
			u->cancelTrain();
		}
	}
}

void ProductionManager::UpdateWeightLists()
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

void ProductionManager::SetUnitWeight(UnitType unit, int Weight)
{
	UnitWeights[unit] = Weight;
}

ProductionManager * getProductionManager()
{
	return LocalProductionManager;
}

Unitset ProductionManager::getProductionBuildings()
{
	return assignedUnits;
}

inline void lpmSuw(UnitType type, int weight)
{
	LocalProductionManager->SetUnitWeight(type, weight);
}

void initializeProductionManager()
{
	LocalProductionManager = new ProductionManager;
	addManager(LocalProductionManager);

	/*lpmSuw(UnitTypes::Protoss_Probe, 1);
	lpmSuw(UnitTypes::Protoss_Zealot, 1);
	lpmSuw(UnitTypes::Protoss_Observer, 1);
	lpmSuw(UnitTypes::Protoss_Reaver, 1);
	lpmSuw(UnitTypes::Protoss_Shuttle, 1);
	lpmSuw(UnitTypes::Protoss_Scout, 1);
	lpmSuw(UnitTypes::Protoss_Corsair, 1);
	lpmSuw(UnitTypes::Protoss_Arbiter, 1);
	lpmSuw(UnitTypes::Protoss_Carrier, 1);*/

	LocalProductionManager->UpdateWeightLists();
}