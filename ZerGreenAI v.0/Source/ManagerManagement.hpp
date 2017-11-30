#pragma once

#include <unordered_set>

#include "Namespaces.h"
#include "GeneralManagement.h"

class ManagerManager;

class ChildManager : protected virtual Manager
{
protected:
	Manager * parent;
public:
	inline virtual std::string name() { return "Unnamed Child Manager"; }
	friend ManagerManager;
};

typedef std::unordered_set<ChildManager *> ChildManagerSet;

class ManagerManager : protected virtual Manager
{
protected:
	inline void addManager(ChildManager * mgr) { childManagers.insert(mgr); mgr->parent = this; };
	ChildManagerSet childManagers;
public:
	~ManagerManager();
	inline virtual std::string name() { return "Unnamed Manager Manager"; }
};
