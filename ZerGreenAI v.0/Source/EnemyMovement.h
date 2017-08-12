#pragma once
#include "Namespaces.h"
#include "GeneralManagement.h"

class EnemyMovementManager : Manager
{
	void onUnitDiscover(Unit u);
	void onFrame();
	void onUnitDestroy(Unit u);
	inline virtual std::string name() { return "Enemy Movement Manager"; }

};

class UnitTracker
{
	Unit whatUnit;
	void update();
	void draw();

public:
	UnitType type;
	int id;
	Position position;

	UnitTracker(Unit u);
	friend EnemyMovementManager;
};

EnemyMovementManager * getEnemyMovementManager();