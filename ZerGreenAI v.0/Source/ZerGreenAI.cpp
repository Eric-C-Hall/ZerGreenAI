#include "ZerGreenAI.h"
#include <iostream>

#include "bwemL.h"
#include "GeneralManagement.h"
#include "LocalHarvesting.h"
#include "Namespaces.h"
#include "Debug.h"
#include "Cheats.h"
#include "Timer.h"
#include "MapAnalyser.h"
#include "BuildOrder.h"
#include "LayoutPlanner.h"
#include "BM.h"
#include "Easter.h"
#include "ProbeScout.h"
#include "ScoutAnalysis.h"
#include "CombatStrategist.h"
#include "Control.h"

// ------------
// ZerGreenAI
// ------------
//
// Written by Eric Hall
//
// Uses the BWAPI & BWEM libraries

void ZerGreenAI::onStart()
{

	try
	{
		onStartTimerStart("Total");

		// Enable the UserInput flag, which allows us to control the bot and type messages.
		Broodwar->enableFlag(Flag::UserInput);

		Broodwar->setLatCom(false);
		Broodwar->setFrameSkip(0);
		Broodwar->setGUI(true);

		// Sets the number of milliseconds Broodwar spends in each frame.
		// "Fastest" game speed is 42ms/frame
		Broodwar->setLocalSpeed(5);

		// Set the command optimization level so that common commands can be grouped
		// and reduce the bot's APM (Actions Per Minute).
		Broodwar->setCommandOptimizationLevel(2);

		Broodwar << "------------" << std::endl;
		Broodwar << "ZerGreenAI" << std::endl;
		Broodwar << "------------" << std::endl;
		Broodwar << "Written by ZerGreenOne" << std::endl;
		Broodwar << "Uses the BWEM library" << std::endl;
		Broodwar << "Uses imp42's scouting algorithm's concept" << std::endl;

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

		onStartTimerStart("Map Analyser");
		initializeMapAnalyser();
		onStartTimerEnd("Map Analyser");

		onStartTimerStart("Build Order");
		initializeBuildOrder();
		onStartTimerEnd("Build Order");

		Manager::globalOnStart();

		onStartTimerEnd("Total");
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
}

void ZerGreenAI::onEnd(bool isWinner)
{
	Manager::globalOnEnd(isWinner);
}

void ZerGreenAI::onFrame()
{
	try
	{
		startTimer("Total");
		carriageReturn();

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
		startTimer("Build Order");
		buildOrderOnFrame();
		endTimer("Build Order");
		startTimer("debug drawing");
		debugOnFrame();
		endTimer("debug drawing");
		startTimer("Happy Easter!");
		onFrameEaster();
		endTimer("Happy Easter!");
		cheatOnFrame();
		endTimer("Total");
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
}

void ZerGreenAI::onSendText(std::string text)
{
	try
	{
		if (!executeCheat(text))
		{
			// Send the text to the game if it is not being processed.
			Broodwar->sendText("%s", text.c_str());
		}

		// Make sure to use %s and pass the text as a parameter,
		// otherwise you may run into problems when you use the %(percent) character!

		Manager::globalOnSendText(text);
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}

}

void ZerGreenAI::onReceiveText(BWAPI::Player player, std::string text)
{
	Manager::globalOnReceiveText(player, text);
	if (standardMessages(text))
	{
		return;
	}
}

void ZerGreenAI::onPlayerLeft(BWAPI::Player player)
{
	Manager::globalOnPlayerLeft(player);
}

void ZerGreenAI::onNukeDetect(BWAPI::Position target)
{
	Manager::globalOnNukeDetect(target);
}

void ZerGreenAI::onUnitDiscover(BWAPI::Unit unit)
{
	scoutAnalysisOnDiscover(unit);
	Manager::globalOnUnitDiscover(unit);
}

void ZerGreenAI::onUnitEvade(BWAPI::Unit unit)
{
	Manager::globalOnUnitEvade(unit);
}

void ZerGreenAI::onUnitShow(BWAPI::Unit unit)
{
	Manager::globalOnUnitShow(unit);
}

void ZerGreenAI::onUnitHide(BWAPI::Unit unit)
{
	Manager::globalOnUnitHide(unit);
}

void ZerGreenAI::onUnitCreate(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		getResourceAllocator()->giveOrphanUnit(unit);
	}

	buildOrderOnCreate(unit);

	Manager::globalOnUnitCreate(unit);
}

void ZerGreenAI::onUnitDestroy(BWAPI::Unit unit)
{
	try
	{
		if (unit->getType().isMineralField())    theMap.OnMineralDestroyed(unit);
		else if (unit->getType().isSpecialBuilding()) theMap.OnStaticBuildingDestroyed(unit);

		if (unit->getPlayer() == Broodwar->self())
		{
			recycleUnitSenior(unit);
		}
		Manager::globalOnUnitDestroy(unit);
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION: " << e.what() << std::endl;
	}
}

void ZerGreenAI::onUnitMorph(BWAPI::Unit unit)
{
	if (IsRefinery(unit))
	{
		buildOrderOnCreate(unit);
	}
	Manager::globalOnUnitMorph(unit);
}

void ZerGreenAI::onUnitRenegade(BWAPI::Unit unit)
{
	if (unit->getPlayer() != Broodwar->self())
	{
		recycleUnitSenior(unit);
	}
	Manager::globalOnUnitRenegade(unit);
}

void ZerGreenAI::onSaveGame(std::string gameName)
{
	Manager::globalOnSaveGame(gameName);
}

void ZerGreenAI::onUnitComplete(BWAPI::Unit unit)
{
	Manager::globalOnUnitComplete(unit);
}
