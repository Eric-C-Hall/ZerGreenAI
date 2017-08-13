#pragma once

#include "Namespaces.h"
#include "ZerGreenAI.h"

class Manager
{
	virtual void onStart() {};
	virtual void onEnd(bool isWinner) {};
	virtual void onFrame() {};
	virtual void onSendText(std::string text) {};
	virtual void onReceiveText(Player player, std::string text) {};
	virtual void onPlayerLeft(Player player) {};
	virtual void onNukeDetect(Position target) {};
	virtual void onUnitDiscover(Unit unit) {};
	virtual void onUnitEvade(Unit unit) {};
	virtual void onUnitShow(Unit unit) {};
	virtual void onUnitHide(Unit unit) {};
	virtual void onUnitCreate(Unit unit) {};
	virtual void onUnitDestroy(Unit unit) {};
	virtual void onUnitMorph(Unit unit) {};
	virtual void onUnitRenegade(Unit unit) {};
	virtual void onSaveGame(std::string gameName) {};
	virtual void onUnitComplete(Unit unit) {};

	inline virtual std::string name() { return "Unnamed Manager"; }
	static std::unordered_set<Manager *> managers;

protected:
	bool cleanMeUp = false;
public:

	Manager();
	~Manager();

	static void globalOnStart();
	static void globalOnEnd(bool isWinner);
	static void globalOnFrame();
	static void globalOnSendText(std::string text);
	static void globalOnReceiveText(Player player, std::string text);
	static void globalOnPlayerLeft(Player player);
	static void globalOnNukeDetect(Position target);
	static void globalOnUnitDiscover(Unit unit);
	static void globalOnUnitEvade(Unit unit);
	static void globalOnUnitShow(Unit unit);
	static void globalOnUnitHide(Unit unit);
	static void globalOnUnitCreate(Unit unit);
	static void globalOnUnitDestroy(Unit unit);
	static void globalOnUnitMorph(Unit unit);
	static void globalOnUnitRenegade(Unit unit);
	static void globalOnSaveGame(std::string gameName);
	static void globalOnUnitComplete(Unit unit);


};