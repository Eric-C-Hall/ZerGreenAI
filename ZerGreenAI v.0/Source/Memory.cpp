#include "Memory.h"

MemoryDatum::MemoryDatum(Unit u)
{
	type = u->getType();
	id = u->getID();
}

std::unordered_set memories;

void memoryOnDiscover()
{

}

void memoryOnHide()
{

}

void memoryOnFrame();