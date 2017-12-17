#pragma once

#include <BWAPI.h>

#include "GeneralManagement.hpp"
#include "BuildOrder.hpp"

namespace ZerGreenAI
{
	// Only really need to build 1 thing per frame, that's plenty fast enough.
	// We first ensure that we have built whatever we have decided on.
	// If anything has been decided on, we wait until it has been produced to decide on something new
	// We then decide what we want to build, in the order Units, Upgrades, Buildings
	// We can only decide to build 1 thing, maximum, returning once we have decided.
	class Spender : Manager
	{
		bool makingUnit = false;
		BWAPI::UnitType makeUnitType;
		bool makingBuilding = false;
		BWAPI::UnitType makeBuildingType;

		inline std::string getBOFilePath();

		BuildOrder buildOrder = BuildOrder(getBOFilePath());

		static constexpr int FRAMES_BETWEEN_SPENDING = 1;
		void onStart() override;
		void onFrame() override;
		void onSendText(std::string text) override;
		void onEnd(bool didWin) override;
	public:
		inline virtual std::string name() { return "Spender"; }
	};
}

inline std::string ZerGreenAI::Spender::getBOFilePath()
{
	return "bwapi-data/read/buildOrder.txt";
}