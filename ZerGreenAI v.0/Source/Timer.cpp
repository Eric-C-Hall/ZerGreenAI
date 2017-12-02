#include "stdafx.h"

#include "Namespaces.hpp"
#include "Timer.hpp"
#include "Debug.hpp"

#include <chrono>

using namespace std::chrono;

int currentLine = 0;

std::unordered_map<std::string, std::chrono::milliseconds> string2Time;

void ZerGreenAI::carriageReturn()
{
	currentLine = 0;
}

void ZerGreenAI::startTimer(std::string timer)
{
	string2Time[timer] = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

void ZerGreenAI::endTimer(std::string timer)
{
	milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	Broodwar->drawTextScreen(Position(5, 10 * currentLine), "%s: %d", timer.c_str(), currentTime - string2Time[timer]);
	currentLine++;
	if (currentTime - string2Time[timer] > (milliseconds)85)
	{
		Broodwar << "Timer longer than 85 in " << timer << ", " << std::to_string((currentTime - string2Time[timer]).count()) << std::endl;
	}
}

void ZerGreenAI::onStartTimerStart(std::string timer)
{
	string2Time[timer] = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

void ZerGreenAI::onStartTimerEnd(std::string timer)
{
	milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	std::string output = timer;
	output.append(": ").append(std::to_string((currentTime - string2Time[timer]).count()));

	new debugText(CoordinateType::Screen, 200, 10 * currentLine, output,480);
	currentLine++;
	if (currentTime - string2Time[timer] > (milliseconds)5000)
	{
		Broodwar << "Timer longer than 5000 in " << timer << ", " << std::to_string((currentTime - string2Time[timer]).count()) << std::endl;
	}
	string2Time.erase(timer);
}