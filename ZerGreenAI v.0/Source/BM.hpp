#pragma once

namespace ZerGreenAI
{

#define NUM_TERRAN_WARCRIES 2
#define NUM_ZERG_WARCRIES 2
#define NUM_PROTOSS_WARCRIES 2

	//std::string TerranWarcries[NUM_TERRAN_WARCRIES] = { "How about we play a little game of buggers and astronauts. You play the buggers.", "You are about to be beaten by a Third" }
	//std::string ZergWarcries[NUM_ZERG_WARCRIES] = { "Watch out, bugger scum", "I'm sorry, my friend, for what I must do." }
	//std::string ProtossWarcries[NUM_PROTOSS_WARCRIES] = { "The enemy's gate is going down!" }

	void Warcry();

	bool standardMessages(std::string text);
}