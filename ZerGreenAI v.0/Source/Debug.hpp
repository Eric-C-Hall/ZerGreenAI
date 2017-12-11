#pragma once

#include <BWAPI.h>

namespace ZerGreenAI
{

#define NUM_UNIT_DEBUG_TEXT_FRAMES 24

	void debugOnFrame();
	void debugUnitText(BWAPI::Unit u, std::string text);

	class debugDrawing
	{
		virtual void draw() = 0;
	protected:
		BWAPI::CoordinateType::Enum ctype;
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
		BWAPI::Color color;
		bool isSolid;

		void draw();
	public:
		debugBox(BWAPI::CoordinateType::Enum parCtype, int parLeft, int parTop, int parRight, int parBottom, BWAPI::Color parColor, int parFrames, bool parIsSolid = false);
		debugBox(BWAPI::CoordinateType::Enum parCtype, BWAPI::Position topLeft, BWAPI::Position bottomRight, BWAPI::Color parColor, int parFrames, bool parIsSolid = false);

	};

	class debugText : debugDrawing
	{
		int x;
		int y;
		std::string format;

		void draw();
	public:
		debugText(BWAPI::CoordinateType::Enum parCtype, int parX, int parY, std::string parFormat, int parFrames);
		debugText(BWAPI::CoordinateType::Enum parCtype, BWAPI::Position parPos, std::string parFormat, int parFrames);
		debugText(BWAPI::Position parPos, std::string parFormat, int parFrames);
	};

	class debugLine : debugDrawing
	{
		int x1;
		int y1;
		int x2;
		int y2;
		BWAPI::Color color;

		void draw();
	public:
		debugLine(BWAPI::CoordinateType::Enum parCtype, int parX1, int parY1, int parX2, int parY2, BWAPI::Color parColor, int parFrames);
		debugLine(BWAPI::CoordinateType::Enum parCtype, BWAPI::Position parPos1, BWAPI::Position parPos2, BWAPI::Color parColor, int parFrames);
		debugLine(BWAPI::Position parPos1, BWAPI::Position parPos2, BWAPI::Color parColor, int parFrames);
	};

	class debugCircle : debugDrawing
	{
		int x;
		int y;
		int radius;
		BWAPI::Color color;
		bool isSolid;

		void draw();
	public:
		debugCircle(BWAPI::CoordinateType::Enum parCtype, int parX, int parY, int parRadius, BWAPI::Color parColor, int parFrames, bool parIsSolid = false);
		debugCircle(BWAPI::CoordinateType::Enum parCtype, BWAPI::Position parPos, int parRadius, BWAPI::Color parColor, int parFrames, bool parIsSolid = false);
		debugCircle(BWAPI::Position parPos, int parRadius, BWAPI::Color parColor, int parFrames, bool parIsSolid = false);
	};

	class debugDot : debugDrawing
	{
		int x;
		int y;
		BWAPI::Color color;

		void draw();
	public:
		debugDot(BWAPI::CoordinateType::Enum parCtype, int parX, int parY, BWAPI::Color parColor, int parFrames);
	};

}