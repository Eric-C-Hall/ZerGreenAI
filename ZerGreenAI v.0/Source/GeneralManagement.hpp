#pragma once

namespace ZerGreenAI
{

	class Manager
	{
		virtual void onStart() {};
		virtual void onEnd(bool isWinner) {};
		virtual void onFrame() {};
		virtual void onSendText(std::string text) {};
		virtual void onReceiveText(BWAPI::Player player, std::string text) {};
		virtual void onPlayerLeft(BWAPI::Player player) {};
		virtual void onNukeDetect(BWAPI::Position target) {};
		virtual void onUnitDiscover(BWAPI::Unit unit) {};
		virtual void onUnitEvade(BWAPI::Unit unit) {};
		virtual void onUnitShow(BWAPI::Unit unit) {};
		virtual void onUnitHide(BWAPI::Unit unit) {};
		virtual void onUnitCreate(BWAPI::Unit unit) {};
		virtual void onUnitDestroy(BWAPI::Unit unit) {};
		virtual void onUnitMorph(BWAPI::Unit unit) {};
		virtual void onUnitRenegade(BWAPI::Unit unit) {};
		virtual void onSaveGame(std::string gameName) {};
		virtual void onUnitComplete(BWAPI::Unit unit) {};

		inline virtual std::string name() { return "Unnamed Manager"; }

#if _DEBUG
		const std::string d_name = name();
#endif

		static std::unordered_set<Manager *> managers;
		static std::unordered_set<Manager *> &ensureManagersCleanedUp();
	protected:

		static std::unordered_set<Manager *> cleanUpList;
	public:

		Manager();
		~Manager();

		static void globalOnStart();
		static void globalOnEnd(bool isWinner);
		static void globalOnFrame();
		static void globalOnSendText(std::string text);
		static void globalOnReceiveText(BWAPI::Player player, std::string text);
		static void globalOnPlayerLeft(BWAPI::Player player);
		static void globalOnNukeDetect(BWAPI::Position target);
		static void globalOnUnitDiscover(BWAPI::Unit unit);
		static void globalOnUnitEvade(BWAPI::Unit unit);
		static void globalOnUnitShow(BWAPI::Unit unit);
		static void globalOnUnitHide(BWAPI::Unit unit);
		static void globalOnUnitCreate(BWAPI::Unit unit);
		static void globalOnUnitDestroy(BWAPI::Unit unit);
		static void globalOnUnitMorph(BWAPI::Unit unit);
		static void globalOnUnitRenegade(BWAPI::Unit unit);
		static void globalOnSaveGame(std::string gameName);
		static void globalOnUnitComplete(BWAPI::Unit unit);


	};

}