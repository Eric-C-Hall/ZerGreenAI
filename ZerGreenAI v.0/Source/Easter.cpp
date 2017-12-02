#include "stdafx.h"

#include "Easter.hpp"
#include "Namespaces.hpp"
#include <queue>
#include <chrono>

#define NUM_LYRICS 38
Unit pylo = nullptr;
std::chrono::seconds startTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
std::string Lyrics[NUM_LYRICS] = {"This is the story of a hero straight and true,", "Young Protoss pylon that had not much to do,", "Pylo was his name, and he stood on the front lines,", "Hoping to hold out,", "Until the end of times", "Young traveller Husky,", "Stayed by Pylo's side", "And wagered with his friends", "If and when they thought he'd die", "Husky was a true believer", "In Pylo's might", "And bet that he would last", "All the way...", "Through the fight...", "Until a terrible terran force of marauders and marines", "Struck poor Pylo down before he could realise his dreams", "Saddened zealots cried, and stalkers stayed up late", "They mourned the fateful time when he died,", "14:08", "Pylo, the pylon", "Was Protoss, through and through,", "Pylo, the pylon", "Was loved, by me and you", "*Instrumental*", "If you ever think that you might be as great as him", "Think twice you lads and ladies, for your chances are quite grim", "Though he lasted but 5 minutes on that cratered map", "Young Pylo changed the way we think of pylons", "They're not crap", "Pylo, the pylon", "Was Protoss, through and through.", "Pylo, the pylon", "Was loved, by me and you", "Pylo, the pylon,", "Was Protoss, through and through", "Pylo, the pylon", "Was loved...", "By Zeratul."};

void ZerGreenAI::makeUnitPylo(Unit u)
{
	pylo = u;
}

void ZerGreenAI::onFrameEaster()
{
	if (pylo != nullptr)
	{
		std::chrono::seconds timeDelta = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()) - startTime;
		Broodwar->drawTextMap(pylo->getPosition(), Lyrics[(timeDelta.count() / 3) % NUM_LYRICS].c_str());
	}

}