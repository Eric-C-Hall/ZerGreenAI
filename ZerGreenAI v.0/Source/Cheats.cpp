#include "stdafx.h"

#include "Cheats.hpp"
#include "Namespaces.hpp"

int timeGatheringFrames = -1;
int timeGatheringInitMin;

#define NUM_TIME_GATHERING_FRAMES 1000

bool ZerGreenAI::executeCheat(std::string text)
{
	if (text == "timeGathering")
	{
		Broodwar << "start" << std::endl;
		Broodwar->setFrameSkip(2);
		Broodwar->setLocalSpeed(2);
		timeGatheringFrames = NUM_TIME_GATHERING_FRAMES;
		timeGatheringInitMin = Broodwar->self()->minerals();
		return true;
	}
	if (text == "")
	{

	}
	return false;
}

void ZerGreenAI::cheatOnFrame()
{
	if (timeGatheringFrames >= 0)
	{
		timeGatheringFrames--;

		if (timeGatheringFrames == 0)
		{
			Broodwar->setFrameSkip(1);
			Broodwar->setLocalSpeed(24);
			Broodwar << ((Broodwar->self()->minerals() - timeGatheringInitMin)) << std::endl;
		}
	}
}