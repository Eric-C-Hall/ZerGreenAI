#pragma once

#include "ManagerManagement.hpp"
#include "UnitManagement.h"

class MacroCombatManager : protected virtual ManagerManager, protected virtual UnitManager
{
	void onFrame();
public:
	inline virtual std::string name() { return "Macro Combat Manager"; }
};