#include "EventExt.h"
#include "GeneralManagement.h"

std::unordered_set<EventExt *> events;

EventExt::EventExt()
{
	events.insert(this);
}

EventExt::~EventExt()
{
	events.erase(this);
}