#pragma once
#include <BWAPI.h>

// Remember not to use "Broodwar" in any global class constructor!

#define ZGA_TRIANGULAR_GRID_SIZE 75

namespace ZerGreenAI
{
	class ResourceAllocator;
	class GlobalHarvestManager;
	class LayoutPlanner;
	class ConstructionManager;
	class ProductionManager;
	class UpgradeManager;
	class EnemyMovementManager;
	class IMPScoutManager;
	class CombatStrategist;
	class MacroCombatManager;
	class MacroCombatManager;
	class SaveLoadManager;
	class BuildOrderManager;
	class PylonConstructionManager;

	template<int distance>
	class TriangularGrid;

	class ZerGreenAIObj : public BWAPI::AIModule
	{
		static int numInstances;
	public:

		bool drawTriangleGrid = false;
		bool drawDebugTimers = false;

		static ZerGreenAIObj * mainInstance;

		ResourceAllocator * resourceAllocator;
		GlobalHarvestManager * globalHarvestManager;
		LayoutPlanner * layoutPlanner;
		ConstructionManager * constructionManager;
		ProductionManager * productionManager;
		UpgradeManager * upgradeManager;
		EnemyMovementManager * enemyMovementManager;
		IMPScoutManager * impScoutManager;
		CombatStrategist * combatStrategist;
		MacroCombatManager * macroCombatManager;
		SaveLoadManager * saveLoadManager;
		BuildOrderManager * buildOrderManager;
		PylonConstructionManager * pylonConstructionManager;

		TriangularGrid<ZGA_TRIANGULAR_GRID_SIZE> * grid;
		std::vector<BWAPI::Position> findPath(BWAPI::Position a, BWAPI::Position b);

		// Virtual functions for callbacks, leave these as they are.
		virtual void onStart();
		virtual void onEnd(bool isWinner);
		virtual void onFrame();
		virtual void onSendText(std::string text);
		virtual void onReceiveText(BWAPI::Player player, std::string text);
		virtual void onPlayerLeft(BWAPI::Player player);
		virtual void onNukeDetect(BWAPI::Position target);
		virtual void onUnitDiscover(BWAPI::Unit unit);
		virtual void onUnitEvade(BWAPI::Unit unit);
		virtual void onUnitShow(BWAPI::Unit unit);
		virtual void onUnitHide(BWAPI::Unit unit);
		virtual void onUnitCreate(BWAPI::Unit unit);
		virtual void onUnitDestroy(BWAPI::Unit unit);
		virtual void onUnitMorph(BWAPI::Unit unit);
		virtual void onUnitRenegade(BWAPI::Unit unit);
		virtual void onSaveGame(std::string gameName);
		virtual void onUnitComplete(BWAPI::Unit unit);
		// Everything below this line is safe to modify.

		ZerGreenAIObj();
		~ZerGreenAIObj();

	};

}