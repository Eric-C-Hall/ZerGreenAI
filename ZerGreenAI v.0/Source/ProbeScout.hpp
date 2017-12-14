#pragma once

#include "UnitManagement.hpp"

namespace ZerGreenAI
{

	class ProbeScoutManager : public UnitManager
	{
		const int CHATTER_MAX_DURATION = 300;
		const int CHATTER_MIN_DURATION = 48;
		std::vector<std::string> chatter = { "doo, doo-doo", "dee-dah day", "hmmmm", "whats this?", "whats over here", "la de dah", "...", "???", "!!!", "I wonder how the probe count is doing back home", "*picks nose*", "*does a barrel roll*", "oh, what a nice-looking rock", "why am I always the one chosen for the scouting missions?", ":(", "*beep*", "*boop*", "*whistle", "*chatter*", "*squeal*" };
		int chatterTime = 0;
		int chatterDuration = 100;
		std::string currentChatter = "Oh god, I'm going out scouting";
		void onAssignment(BWAPI::Unit u) override;
		void onReassignment(BWAPI::Unit u) override;
	public:
		inline virtual std::string name() { return "Probe Scout Manager"; }
		void onFrame() override;
	};

	void startProbeScout();

}