#pragma once

#include "Namespaces.h"

void debugOnFrame();

class debugDrawing
{
	virtual void draw() = 0;
protected:
	CoordinateType::Enum ctype;
	int frames;
public:
	debugDrawing();
	~debugDrawing();
	void drawFrame();
};


class debugBox : debugDrawing
{
	int left;
	int right;
	int top;
	int bottom;
	Color color;
	bool isSolid;

	void draw();
public:
	debugBox(CoordinateType::Enum parCtype, int parLeft, int parRight, int parTop, int parBottom, Color parColor, int parFrames, bool parIsSolid = false);
};

class debugText : debugDrawing
{
	int x;
	int y;
	std::string format;

	void draw();
public:
	debugText(CoordinateType::Enum parCtype, int parX, int parY, std::string parFormat, int parFrames);
	debugText(CoordinateType::Enum parCtype, Position parPos, std::string parFormat, int parFrames);
	debugText(Position parPos, std::string parFormat, int parFrames);
};

class debugLine : debugDrawing
{
	int x1;
	int y1;
	int x2;
	int y2;
	Color color;

	void draw();
public:
	debugLine(CoordinateType::Enum parCtype, int parX1, int parY1, int parX2, int parY2, Color parColor, int parFrames);
	debugLine(CoordinateType::Enum parCtype, Position parPos1, Position parPos2, Color parColor, int parFrames);
	debugLine(Position parPos1, Position parPos2, Color parColor, int parFrames);
};

class debugCircle : debugDrawing
{
	int x;
	int y;
	int radius;
	Color color;
	bool isSolid;

	void draw();
public:
	debugCircle(CoordinateType::Enum parCtype, int parX, int parY, int parRadius, Color parColor, int parFrames, bool parIsSolid = false);
	debugCircle(CoordinateType::Enum parCtype, Position parPos, int parRadius, Color parColor, int parFrames, bool parIsSolid = false);
	debugCircle(Position parPos, int parRadius, Color parColor, int parFrames, bool parIsSolid = false);
};

class debugDot : debugDrawing
{
	int x;
	int y;
	Color color;

	void draw();
public:
	debugDot(CoordinateType::Enum parCtype, int parX, int parY, Color parColor, int parFrames);
};