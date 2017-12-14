#include "stdafx.h"

#include "Debug.hpp"
#include "Namespaces.hpp"
#include "UnitManagement.hpp"

std::unordered_map<Unit, std::string> unit2DebugText;
std::unordered_map<Unit, int> debugTextFrames;
std::unordered_set<debugDrawing *> drawings;
std::unordered_set<debugDrawing *> removedDrawings;

void ZerGreenAI::debugOnFrame()
{
	Unitset eraseDebugTextUnits;
	for (auto const &d : unit2DebugText)
	{
		if (debugTextFrames[d.first] <= 1) eraseDebugTextUnits.insert(d.first);
		else debugTextFrames[d.first]--;

		Broodwar->drawTextMap(d.first->getPosition(), d.second.c_str());
	}
	for (auto const &u : eraseDebugTextUnits)
	{
		unit2DebugText.erase(u);
		debugTextFrames.erase(u);
	}

	// Important: Debug objects should be created using the 'new' keyword.
	removedDrawings.clear();
	for (auto const &d : drawings)
	{
		d->drawFrame();
	}
	for (auto const &d : removedDrawings)
	{
		drawings.erase(d);
		delete d;
	}

	if (Broodwar->getSelectedUnits().size() == 1)
	{
		Unit selectedUnit = *Broodwar->getSelectedUnits().begin();
		if (selectedUnit != nullptr) // Should never be nullptr, but I think it was, once
		{
			Broodwar->drawTextMap(selectedUnit->getPosition() + Position(0, 20), selectedUnit->getOrder().c_str());

			if (getUnitManager(selectedUnit) == nullptr)
			{
				Broodwar->drawTextMap(selectedUnit->getPosition() + Position(0, 30), "Nullptr");
			}
			else if (getUnitManager(selectedUnit) == (void *)0xcdcdcdcd)
			{
				Broodwar->drawTextMap(selectedUnit->getPosition() + Position(0, 30), "0xcdcdcdcd");
			}
			else
			{
				Broodwar->drawTextMap(selectedUnit->getPosition() + Position(0, 30), getUnitManager(selectedUnit)->name().c_str());
			}
		}
	}
}

void ZerGreenAI::debugUnitText(Unit u, std::string text, int numFrames)
{
	unit2DebugText[u] = text;
	debugTextFrames[u] = numFrames;
}

debugDrawing::debugDrawing()
{
	drawings.insert(this);
}

debugDrawing::~debugDrawing()
{
	if (this->frames > 0)
	{
		Broodwar << "Warning: debug drawing destroyed by outside causes" << std::endl << "Probably not created using new" << std::endl;
	}
	drawings.erase(this);
}

void debugDrawing::drawFrame()
{
	// Important: Debug objects should be created using the 'new' keyword.
	frames--;
	if (frames <= 0)
	{
		removedDrawings.insert(this);
	}
	draw();
}

void debugBox::draw()
{
	Broodwar->drawBox(ctype, left, top, right, bottom, color, isSolid);
}

debugBox::debugBox(CoordinateType::Enum parCtype, int parLeft, int parTop, int parRight, int parBottom, Color parColor, int parFrames, bool parIsSolid)
{
	ctype = parCtype;
	left = parLeft;
	right = parRight;
	top = parTop;
	bottom = parBottom;
	color = parColor;
	isSolid = parIsSolid;
	frames = parFrames;
}

ZerGreenAI::debugBox::debugBox(BWAPI::CoordinateType::Enum parCtype, BWAPI::Position topLeft, BWAPI::Position bottomRight, BWAPI::Color parColor, int parFrames, bool parIsSolid) : debugBox(parCtype, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, parColor, parFrames, parIsSolid)
{
}


void debugText::draw()
{
	Broodwar->drawText(ctype, x, y, format.c_str());
}

debugText::debugText(CoordinateType::Enum parCtype, int parX, int parY, std::string parFormat, int parFrames)
{
	ctype = parCtype;
	x = parX;
	y = parY;
	format = parFormat;
	frames = parFrames;
}

debugText::debugText(CoordinateType::Enum parCtype, Position parPos, std::string parFormat, int parFrames)
{
	ctype = parCtype;
	x = parPos.x;
	y = parPos.y;
	format = parFormat;
	frames = parFrames;
}

debugText::debugText(Position parPos, std::string parFormat, int parFrames)
{
	ctype = CoordinateType::Map;
	x = parPos.x;
	y = parPos.y;
	format = parFormat;
	frames = parFrames;
}

void debugLine::draw()
{
	Broodwar->drawLine(ctype, x1, y1, x2, y2, color);
}

debugLine::debugLine(CoordinateType::Enum parCtype, int parX1, int parY1, int parX2, int parY2, Color parColor, int parFrames)
{
	ctype = parCtype;
	x1 = parX1;
	y1 = parY1;
	x2 = parX2;
	y2 = parY2;
	color = parColor;
	frames = parFrames;
}

debugLine::debugLine(CoordinateType::Enum parCtype, Position parPos1, Position parPos2, Color parColor, int parFrames)
{
	ctype = parCtype;
	x1 = parPos1.x;
	y1 = parPos1.y;
	x2 = parPos2.x;
	y2 = parPos2.y;
	color = parColor;
	frames = parFrames;
}

debugLine::debugLine(Position parPos1, Position parPos2, Color parColor, int parFrames)
{
	ctype = CoordinateType::Map;
	x1 = parPos1.x;
	y1 = parPos1.y;
	x2 = parPos2.x;
	y2 = parPos2.y;
	color = parColor;
	frames = parFrames;
}

void debugCircle::draw()
{
	Broodwar->drawCircle(ctype, x, y, radius, color, isSolid);
}

debugCircle::debugCircle(CoordinateType::Enum parCtype, int parX, int parY, int parRadius, Color parColor, int parFrames, bool parIsSolid)
{
	ctype = parCtype;
	x = parX;
	y = parY;
	radius = parRadius;
	color = parColor;
	frames = parFrames;
	isSolid = parIsSolid;
}

debugCircle::debugCircle(CoordinateType::Enum parCtype, Position parPos, int parRadius, Color parColor, int parFrames, bool parIsSolid)
{
	ctype = parCtype;
	x = parPos.x;
	y = parPos.y;
	radius = parRadius;
	color = parColor;
	frames = parFrames;
	isSolid = parIsSolid;
}

debugCircle::debugCircle( Position parPos, int parRadius, Color parColor, int parFrames, bool parIsSolid)
{
	ctype = CoordinateType::Map;
	x = parPos.x;
	y = parPos.y;
	radius = parRadius;
	color = parColor;
	frames = parFrames;
	isSolid = parIsSolid;
}

void debugDot::draw()
{
	Broodwar->drawDot(ctype, x, y, color);
}

debugDot::debugDot(CoordinateType::Enum parCtype, int parX, int parY, Color parColor, int parFrames)
{
	ctype = parCtype;
	x = parX;
	y = parY;
	color = parColor;
	frames = parFrames;
}

ZerGreenAI::debugDot::debugDot(BWAPI::Position p, BWAPI::Color parColor, int parFrames) : debugDot(BWAPI::CoordinateType::Map, p.x, p.y, parColor, parFrames)
{
}
