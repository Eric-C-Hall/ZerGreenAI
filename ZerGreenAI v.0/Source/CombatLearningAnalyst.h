#pragma once
#include "Namespaces.h"

namespace combatLearning
{
	void onStart();

	void addData(Unitset attackers, Unitset defenders, bool didWin);

	void appendData();
	void saveData();
	void loadData();

	bool guessIfWin(Unitset attackers, Unitset defenders);

}