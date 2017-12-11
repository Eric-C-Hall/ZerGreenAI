#include "stdafx.h"

#include <fstream>

#include "ZerGreenAI.hpp"
#include "BuildOrderOld.hpp"
#include "Hashes.hpp"
#include "Production.hpp"
#include "Upgrade.hpp"
#include "ProbeScout.hpp"
#include "Namespaces.hpp"

struct BuildOrderFile;
template <>
struct std::hash<BuildOrderFile>;
std::unordered_map<Race,std::vector<BuildOrderFile>> potentialBuildOrders;
std::unordered_map<UnitType, int> unitTypeWeight;

struct BuildOrderFile
{
	Race race;
	std::string filePath;
};

enum BuildOrderCommand
{
	none = 0,
	scout
};

struct BuildOrderElement
{
	UnitType unit = UnitTypes::None;
	TechType tech = TechTypes::None;
	UpgradeType upgrade = UpgradeTypes::None;
	BuildOrderCommand command = none;
};

std::vector<BuildOrderElement> buildOrder;
auto buildOrderIterator = buildOrder.begin();

namespace std
{
	template<>
	struct hash<BuildOrderFile>
	{
		size_t operator()(BuildOrderFile file)
		{
			return std::hash<std::string>()(file.filePath) + file.race;
		}
	};
}

void saveBuildOrder(std::string filePath)
{
	filePath.insert(0, "bwapi-data/write/buildOrders/");

	std::ofstream saveFile;
	saveFile.open(filePath);
	saveFile.clear();

	if (!saveFile.is_open()) {
		Broodwar << "File could not be opened" << std::endl;
		return;
	}

	int convertEnum;
	for (auto const &elt : buildOrder)
	{
		if (elt.unit != UnitTypes::None)
		{
			saveFile << 'U';
			convertEnum = elt.unit;
			saveFile << convertEnum;
		}
		else if (elt.tech != TechTypes::None)
		{
			saveFile << 'T';
			convertEnum = elt.tech;
			saveFile << convertEnum;
		}
		else if (elt.upgrade != UpgradeTypes::None)
		{
			saveFile << 'G';
			convertEnum = elt.upgrade;
			saveFile << convertEnum;
		}
		else if (elt.command != none)
		{
			saveFile << 'C';
			convertEnum = elt.command;
			saveFile << convertEnum;
		}
	}
	saveFile << 'X';
	saveFile << '0';
	saveFile.close();
}

void loadBuildOrder(std::string filePath)
{
	filePath.insert(0, "bwapi-data/read/buildOrders/");

	std::ifstream loadFile;
	loadFile.open(filePath);

	if (!loadFile.is_open()) {
		Broodwar << "File could not be opened" << std::endl;
		return;
	}

	buildOrder.clear();

	char eltType;
	int convertEnum;
	BuildOrderElement tempElement;

	while (!loadFile.bad())
	{
		loadFile >> eltType;
		loadFile >> convertEnum;

		if (eltType == 'U')
		{
			tempElement.unit = convertEnum;
			tempElement.tech = TechTypes::None;
			tempElement.upgrade = UpgradeTypes::None;
			tempElement.command = none;
		}
		else if (eltType == 'T')
		{
			tempElement.tech = convertEnum;
			tempElement.unit = UnitTypes::None;
			tempElement.upgrade = UpgradeTypes::None;
			tempElement.command = none;
		}
		else if (eltType == 'G')
		{
			tempElement.upgrade = convertEnum;
			tempElement.unit = UnitTypes::None;
			tempElement.tech = TechTypes::None;
			tempElement.command = none;
		}
		else if (eltType == 'C')
		{
			tempElement.command = (BuildOrderCommand)convertEnum;
			tempElement.unit = UnitTypes::None;
			tempElement.tech = TechTypes::None;
			tempElement.upgrade = UpgradeTypes::None;
		}
		else if (eltType == 'X')
		{
			break;
		}

		buildOrder.insert(buildOrder.end(), tempElement); 
	}
	buildOrderIterator = buildOrder.begin();
	assert(!buildOrder.empty());
	Broodwar << "Successfully loaded " << filePath << std::endl;
	loadFile.close();
}

#define SUPPLY_LEEWAY ( Broodwar->self()->supplyTotal() < 25 ? 4 : 18)

bool pylonNeccessary(); // Scavenged for PylonConstruction

void ZerGreenAI::buildOrderOnCreate(Unit unit)
{
	if (buildOrderIterator != buildOrder.end() && unit->getType() == buildOrderIterator->unit)
	{
		buildOrderIterator++;
	}
}

void ZerGreenAI::buildOrderOnFrame()
{
	if (Broodwar->getFrameCount() % 20 != 0)
	{
		return;
	}

	if (pylonNeccessary() && Broodwar->canMake(UnitTypes::Protoss_Pylon))
	{
		ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(UnitTypes::Protoss_Pylon);
		return;
	}




	if (buildOrderIterator == buildOrder.end())
	{
		static bool hasReachedEnd = false;
		if (!hasReachedEnd)
		{
			hasReachedEnd = true;
		}
		return;
	}

	if (buildOrderIterator->unit != UnitTypes::None)
	{
		UnitType chosenType = buildOrderIterator->unit;
		if (chosenType.isBuilding())
		{
			if (Broodwar->canMake(chosenType))
			{
				ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(chosenType);
			}
		}
		else
		{
			ZerGreenAIObj::mainInstance->productionManager->SetUnitWeight(chosenType, ++unitTypeWeight[chosenType]);
			ZerGreenAIObj::mainInstance->productionManager->UpdateWeightLists();
			buildOrderIterator++;

		}

		return;
	}
	else if (buildOrderIterator->tech != TechTypes::None)
	{
		ZerGreenAIObj::mainInstance->upgradeManager->ChooseTech(buildOrderIterator->tech);
		buildOrderIterator++;
	}
	else if (buildOrderIterator->upgrade != UpgradeTypes::None)
	{
		ZerGreenAIObj::mainInstance->upgradeManager->ChooseUpgrade(buildOrderIterator->upgrade);
		buildOrderIterator++;
	}
	else if (buildOrderIterator->command != none)
	{
		if (buildOrderIterator->command == scout)
		{
			startProbeScout();
		}
		buildOrderIterator++;
	}
}

void readBuildOrderManager()
{
	std::ifstream loadFile;
	loadFile.open("bwapi-data/read/buildOrderManager.txt");

	if (!loadFile.is_open()) {
		Broodwar << "File could not be opened" << std::endl;
		return;
	}

	char race;
	std::string fileName;

	while (!loadFile.bad())
	{
		BuildOrderFile currentFile;

		loadFile >> race;
		loadFile >> fileName;
		currentFile.filePath = fileName;

		switch (race)
		{
		case 'T': currentFile.race = Races::Terran; break;
		case 'Z': currentFile.race = Races::Zerg; break;
		case 'P': currentFile.race = Races::Protoss; break;
		case 'R': currentFile.race = Races::Unknown; break;
		case 'X': goto gotFileNames; break;
		}

		auto buildVector = &potentialBuildOrders[currentFile.race];
		buildVector->insert(buildVector->end(), currentFile);
	}
gotFileNames:;
}


std::string chooseBuildOrder()
{
	assert(!potentialBuildOrders[Broodwar->enemy()->getRace()].empty());

	const auto &buildVector = potentialBuildOrders[Broodwar->enemy()->getRace()];
	return buildVector[rand() % buildVector.size()].filePath;
}

void ZerGreenAI::initializeBuildOrder()
{
	readBuildOrderManager();
	loadBuildOrder(chooseBuildOrder());
	saveBuildOrder(chooseBuildOrder());
	//checkCompletedPartsOfBuildOrder();

}