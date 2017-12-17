#include "stdafx.h"

#include "Easter.hpp"
#include "Namespaces.hpp"
#include <queue>
#include <chrono>

#define NUM_LYRICS 38
Unit pylo = nullptr;
std::chrono::seconds startTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
std::string Lyrics[NUM_LYRICS] = {"REDACTED"};

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