#include "EventExt.hpp"
#include "GeneralManagement.hpp"
#include "Namespaces.hpp"

std::unordered_set<EventExt *> events;

EventExt::EventExt()
{
	events.insert(this);
}

EventExt::~EventExt()
{
	events.erase(this);
}