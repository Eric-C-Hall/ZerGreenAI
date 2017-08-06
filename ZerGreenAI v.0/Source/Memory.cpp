#include "Memory.h"

MemoryDatum::MemoryDatum(Unit u)
{
	type = u->getType();
	id = u->getID();
}

namespace std
{
	template<>
	struct hash<MemoryDatum>
	{
		size_t operator()(MemoryDatum datum)
		{
			return datum.id;
		}
	};
}

std::unordered_set<MemoryDatum> memories;

void memoryOnDiscover()
{

}

void memoryOnHide()
{

}

void memoryOnFrame()
{

}