#pragma once
#include "Namespaces.h"
#include "Management.h"

class LayoutPlanner : public JuniorManager
{
	void recycleUnit(Unit u);
	bool isPositionAvailable(TilePosition pos, UnitType type);

public:
	TilePosition getAvailablePosition(UnitType type);
	inline std::string name() { return "Layout Planner"; }
};

class LayoutNode;

namespace std
{
	bool operator==(const LayoutNode &a, const LayoutNode &b);

	template <>
	struct hash<LayoutNode>
	{
		size_t operator()(LayoutNode node) const;
	};
}

class LayoutNode
{
public:
	enum LayoutNodeType { undefined, main, corner, mineralEdge };
	LayoutNodeType nodeType = undefined;

	LayoutNode(){ node = TilePositions::None; }
	LayoutNode(TilePosition pos, LayoutNodeType type){ node = pos; nodeType = type; }

	TilePosition node;
	std::unordered_set<LayoutNode> children;
	void draw();
};

struct LockedPosition
{
	UnitType type;
	TilePosition pos;
};

bool operator==(const LockedPosition &a, const LockedPosition &b);

// Locking is currently unused. The layout is simple and procedurally generated
void lockLayout(UnitType, TilePosition); // Called when a building begins construction
void unlockLayout(UnitType, TilePosition); // Called when a building is destroyed

void drawHighLevelLayout();

void initializeLayoutPlanner();
LayoutPlanner* getLayoutPlanner();