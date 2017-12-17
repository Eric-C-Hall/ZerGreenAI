#pragma once

namespace ZerGreenAI
{

	void carriageReturn();
	void startTimer(std::string timer);
	void endTimer(std::string timer);
	void onStartTimerStart(std::string timer);
	void onStartTimerEnd(std::string timer);
	void startTimerToOut(std::string timer);
	void endTimerToOut(std::string timer);

}