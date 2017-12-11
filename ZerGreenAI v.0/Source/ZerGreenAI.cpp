#include "stdafx.h"

#include "ZerGreenAI.hpp"
#include <iostream>
#include <time.h>

#include "bwemL.hpp"
#include "GeneralManagement.hpp"
#include "LocalHarvesting.hpp"
#include "Namespaces.hpp"
#include "Debug.hpp"
#include "Cheats.hpp"
#include "Timer.hpp"
#include "MapAnalyser.hpp"
#include "LayoutPlanner.hpp"
#include "BM.hpp"
#include "Easter.hpp"
#include "ProbeScout.hpp"
#include "ScoutAnalysis.hpp"
#include "ResourceAllocator.hpp"
#include "CombatStrategist.hpp"
#include "Vector.hpp"
#include "TriangularGrid.hpp"
#include "MacroCombatGroup.hpp"
#include "GlobalHarvesting.hpp"
#include "Production.hpp"
#include "Upgrade.hpp"
#include "EnemyMovement.hpp"
#include "IMPScoutManager.hpp"
#include "Construction.hpp"
#include "BuildOrder.hpp"
#include "PylonConstruction.hpp"

// ------------
// ZerGreenAI
// ------------
//
// Written by Eric Hall
//
// Uses the BWAPI & BWEM libraries

std::vector<BWAPI::Position> ZerGreenAI::ZerGreenAIObj::findPath(BWAPI::Position a, BWAPI::Position b)
{
	return grid->findPath(a, b);
}

int ZerGreenAIObj::numInstances = 0;
ZerGreenAIObj * ZerGreenAIObj::mainInstance = nullptr;

void ZerGreenAIObj::onStart()
{
#if !_DEBUG
	try
	{
#endif
		onStartTimerStart("Total");

		// Enable the UserInput flag, which allows us to control the bot and type messages.
		Broodwar->enableFlag(Flag::UserInput);

		Broodwar->setLatCom(false);
		Broodwar->setFrameSkip(0);
		Broodwar->setGUI(true);

		// Sets the number of milliseconds Broodwar spends in each frame.
		// "Fastest" game speed ises 42ms/frame
		Broodwar->setLocalSpeed(1);

		// Set the command optimization level so that common commands can be grouped
		// and reduce the bot's APM (Actions Per Minute).
		Broodwar->setCommandOptimizationLevel(2);

		Broodwar << "------------" << std::endl;
		Broodwar << "ZerGreenAI" << std::endl;
		Broodwar << "------------" << std::endl;
		Broodwar << "Written by ZerGreenOne" << std::endl;
		Broodwar << "Uses the BWEM library" << std::endl;
		Broodwar << "Uses imp42's scouting algorithm's concept" << std::endl;
		Broodwar << "Uses justinhj's AStar implemenation" << std::endl;
		Broodwar << "Uses N00byEdge's FAP and Modular Neural Networks" << std::endl;
		Broodwar << "------------" << std::endl;

		if (Broodwar->self()->getRace() != Races::Protoss)
		{
			Broodwar << Text::BrightRed << "Wrong race - this is a Protoss bot" << std::endl;
			Broodwar << Text::BrightRed << "Wrong race - this is a Protoss bot" << std::endl;
			Broodwar << Text::BrightRed << "Wrong race - this is a Protoss bot" << std::endl;
			Broodwar << Text::BrightRed << "Wrong race - this is a Protoss bot" << std::endl;
		}

		onStartTimerStart("BWEM Init");
		theMap.Initialize();
		theMap.EnableAutomaticPathAnalysis();
		bool startingLocationsOK = theMap.FindBasesForStartingLocations();
		assert(startingLocationsOK);
		onStartTimerEnd("BWEM Init");

		onStartTimerStart("Create Managers");
		macroCombatManager = new MacroCombatManager;
		combatStrategist = new CombatStrategist;
		globalHarvestManager = new GlobalHarvestManager;
		resourceAllocator = new ResourceAllocator;
		constructionManager = new ConstructionManager;
		layoutPlanner = new LayoutPlanner;
		productionManager = new ProductionManager;
		upgradeManager = new UpgradeManager;
		enemyMovementManager = new EnemyMovementManager;
		impScoutManager = new IMPScoutManager;
		buildOrderManager = new BuildOrderManager;
		pylonConstructionManager = new PylonConstructionManager;
		onStartTimerEnd("Create Managers");

		onStartTimerStart("Map Analyser");
		initializeMapAnalyser();
		onStartTimerEnd("Map Analyser");

		onStartTimerStart("Triangular Grid");
		grid = new TriangularGrid<ZGA_TRIANGULAR_GRID_SIZE>();
		onStartTimerEnd("Triangular Grid");

		Manager::globalOnStart();

		onStartTimerEnd("Total");
#if !_DEBUG
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
#endif
}

void ZerGreenAIObj::onEnd(bool isWinner)
{
	Manager::globalOnEnd(isWinner);
}

template<BWAPI::Key key>
bool keyToggle()
{
	static bool keyWasPressed = false;
	bool keyIsPressed = Broodwar->getKeyState(key);
	if (keyIsPressed && !keyWasPressed)
	{
		keyWasPressed = keyIsPressed;
		return true;
	}
	keyWasPressed = keyIsPressed;
	return false;
}

void ZerGreenAIObj::onFrame()
{
#if !_DEBUG
	try
	{
#endif
		startTimer("Total");
		carriageReturn();

		if (keyToggle<Key::K_G>())
		{
			drawTriangleGrid = !drawTriangleGrid;
		}

		if (keyToggle<Key::K_M>())
		{
			drawDebugTimers = !drawDebugTimers;
		}

		if (drawTriangleGrid)
		{
			startTimer("TriangularGrid");
			grid->draw(Colors::Yellow);
			endTimer("TriangularGrid");
		}


		startTimer("BWEM drawing");

		for (const BWEM::Area & area : theMap.Areas())
		{
			for (const BWEM::ChokePoint * cp : area.ChokePoints())
			{
				Broodwar->drawLineMap(Position(cp->Pos(BWEM::ChokePoint::middle)), Position(cp->Pos(BWEM::ChokePoint::end1)), Colors::White);
				Broodwar->drawLineMap(Position(cp->Pos(BWEM::ChokePoint::middle)), Position(cp->Pos(BWEM::ChokePoint::end2)), Colors::White);
				Broodwar->drawCircleMap(Position(cp->Pos(BWEM::ChokePoint::middle)), 5, Colors::White, true);
			}

		}

		drawHighLevelLayout();

		endTimer("BWEM drawing");
		Manager::globalOnFrame();
		startTimer("debug drawing");
		debugOnFrame();
		endTimer("debug drawing");
		startTimer("Happy Easter!");
		onFrameEaster();
		endTimer("Happy Easter!");

		/*Broodwar->drawCircleMap(grid->snapToGrid(Broodwar->getMousePosition() + Broodwar->getScreenPosition()), 7, Colors::Red, true);
		if (Broodwar->getSelectedUnits().size() > 0)
		{
			Broodwar->drawCircleMap(grid->snapToGrid((*Broodwar->getSelectedUnits().begin())->getPosition()), 9, Colors::Green, true);
		}*/

		cheatOnFrame();
		endTimer("Total");
#if !_DEBUG
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
#endif
}

void ZerGreenAIObj::onSendText(std::string text)
{
#if !_DEBUG
	try
	{
#endif
		if (!executeCheat(text))
		{
			// Send the text to the game if it is not being processed.
			Broodwar->sendText("%s", text.c_str());
		}

		// Make sure to use %s and pass the text as a parameter,
		// otherwise you may run into problems when you use the %(percent) character!

		Manager::globalOnSendText(text);
#if !_DEBUG

	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
#endif
}

void ZerGreenAIObj::onReceiveText(BWAPI::Player player, std::string text)
{
	Manager::globalOnReceiveText(player, text);
	if (standardMessages(text))
	{
		return;
	}
}

void ZerGreenAIObj::onPlayerLeft(BWAPI::Player player)
{
	Manager::globalOnPlayerLeft(player);
}

void ZerGreenAIObj::onNukeDetect(BWAPI::Position target)
{
	Manager::globalOnNukeDetect(target);
}

void ZerGreenAIObj::onUnitDiscover(BWAPI::Unit unit)
{
	scoutAnalysisOnDiscover(unit);
	Manager::globalOnUnitDiscover(unit);
}

void ZerGreenAIObj::onUnitEvade(BWAPI::Unit unit)
{
	Manager::globalOnUnitEvade(unit);
}

void ZerGreenAIObj::onUnitShow(BWAPI::Unit unit)
{
	Manager::globalOnUnitShow(unit);
}

void ZerGreenAIObj::onUnitHide(BWAPI::Unit unit)
{
	Manager::globalOnUnitHide(unit);
}

void ZerGreenAIObj::onUnitCreate(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		ZerGreenAIObj::mainInstance->resourceAllocator->giveOrphanUnit(unit);
	}

	Manager::globalOnUnitCreate(unit);
}

void ZerGreenAIObj::onUnitDestroy(BWAPI::Unit unit)
{
#if !_DEBUG
	try
	{
#endif
		if (unit->getType().isMineralField())    theMap.OnMineralDestroyed(unit);
		else if (unit->getType().isSpecialBuilding()) theMap.OnStaticBuildingDestroyed(unit);

		if (unit->getPlayer() == Broodwar->self())
		{
			recycleUnitSenior(unit);
		}
		Manager::globalOnUnitDestroy(unit);
#if !_DEBUG
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
#endif
}

void ZerGreenAIObj::onUnitMorph(BWAPI::Unit unit)
{
	Manager::globalOnUnitMorph(unit);
}

void ZerGreenAIObj::onUnitRenegade(BWAPI::Unit unit)
{
	if (unit->getPlayer() != Broodwar->self())
	{
		recycleUnitSenior(unit);
	}
	Manager::globalOnUnitRenegade(unit);
}

void ZerGreenAIObj::onSaveGame(std::string gameName)
{
	Manager::globalOnSaveGame(gameName);
}

void ZerGreenAIObj::onUnitComplete(BWAPI::Unit unit)
{
	Manager::globalOnUnitComplete(unit);
}

ZerGreenAI::ZerGreenAIObj::ZerGreenAIObj()
{
	numInstances++;

	if (numInstances > 1)
	{
		Broodwar << BWAPI::Text::BrightRed << "Error: Two or more instances of ZerGreenAIObj have been created" << std::endl;
		delete this;
	}
	else
	{
		mainInstance = this;
	}

	srand((unsigned)time(NULL));
	for (int i = 0; i < rand() % 1001; i++)
	{
		rand();
	}
	
}

ZerGreenAI::ZerGreenAIObj::~ZerGreenAIObj()
{
	numInstances--;
	delete macroCombatManager;
	delete combatStrategist;
	delete globalHarvestManager;
	delete constructionManager;
	delete layoutPlanner;
	delete productionManager;
	delete upgradeManager;
	delete enemyMovementManager;
	delete impScoutManager;
	delete buildOrderManager;
	delete pylonConstructionManager;

	delete resourceAllocator; // Makes sense to be the last to delete, since UnitManagers might give units to resource allocator on delete
}
