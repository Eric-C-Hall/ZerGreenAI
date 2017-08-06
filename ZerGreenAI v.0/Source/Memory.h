#pragma once
#include "Namespaces.h"

struct MemoryDatum
{
	UnitType type;
	int id;

	MemoryDatum(Unit u);
};

void memoryOnDiscover();
void memoryOnHide();