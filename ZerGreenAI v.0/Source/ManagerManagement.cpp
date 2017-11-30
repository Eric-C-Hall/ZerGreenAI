#pragma once

#include "ManagerManagement.hpp"

ManagerManager::~ManagerManager()
{
	for (auto const &m : childManagers)
	{
		delete m;
	}
}
