#include "BM.h"
#include <string>

bool doneGlhf = false;
bool doneGG = false;
bool doneHi = false;
bool doneQuestion = false;

bool thereIs(std::string a, std::string b)
{
	return a.find(b) != std::string::npos;
}

bool standardMessages(std::string text)
{
	// https://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);

	if (!doneGlhf && thereIs(text,"gl hf"))
	{
		Broodwar->sendText("You wish me good luck?? I wish you death.");
		doneGlhf = true;
		return true;
	}

	if (!doneGG && thereIs(text,"gg"))
	{
		Broodwar->sendText("Get rekt you scrubl0rd");
		doneGG = true;
		return true;
	}

	if (!doneHi && (thereIs(text,"hello") || thereIs(text,"hi")))
	{
		Broodwar->sendText("Goodbye.");
		doneHi = true;
		return true;
	}

	if (!doneQuestion && thereIs(text, "???"))
	{
		Broodwar->sendText("wtf are you so confused about????");
		doneQuestion = true;
		return true;
	}
	return false;
}