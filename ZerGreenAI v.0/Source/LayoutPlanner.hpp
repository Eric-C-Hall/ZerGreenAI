#pragma once

#include <BWAPI.h>
#include "UnitManagement.hpp"

namespace ZerGreenAI
{
	class LayoutNode;
	struct LockedPosition;
}

namespace std
{
	bool operator==(const ::ZerGreenAI::LayoutNode &a, const ::ZerGreenAI::LayoutNode &b);
	bool operator==(const ::ZerGreenAI::LockedPosition &a, const ::ZerGreenAI::LockedPosition &b);

	template <>
	struct hash<::ZerGreenAI::LayoutNode>
	{
		size_t operator()(::ZerGreenAI::LayoutNode node) const;
	};
}

namespace ZerGreenAI
{

	class LayoutPlanner : public UnitManager
	{
		void onStart();
		void recycleUnit(BWAPI::Unit u);
		bool isPositionAvailable(BWAPI::TilePosition pos, BWAPI::UnitType type);

	public:
		BWAPI::TilePosition getAvailablePosition(BWAPI::UnitType type);
		inline std::string name() { return "Layout Planner"; }
	};

	class LayoutNode
	{
	public:
		enum LayoutNodeType { undefined, main, corner, mineralEdge };
		LayoutNodeType nodeType = undefined;

		LayoutNode() { node = BWAPI::TilePositions::None; }
		LayoutNode(BWAPI::TilePosition pos, LayoutNodeType type) { node = pos; nodeType = type; }

		BWAPI::TilePosition node;
		std::unordered_set<LayoutNode> children;
		void draw();
	};

	struct LockedPosition
	{
		BWAPI::UnitType type;
		BWAPI::TilePosition pos;
	};

	// Locking is currently unused. The layout is simple and procedurally generated
	void lockLayout(BWAPI::UnitType, BWAPI::TilePosition); // Called when a building begins construction
	void unlockLayout(BWAPI::UnitType, BWAPI::TilePosition); // Called when a building is destroyed

	void drawHighLevelLayout();

	LayoutPlanner * getLayoutPlanner();

}