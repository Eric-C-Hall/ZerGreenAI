#pragma once

#include <BWAPI.h>

#include "Random.hpp"

namespace ZerGreenAI
{

	class BuildOrder
	{
		int currentPosition = 0;
		std::vector<BWAPI::UnitType> currentBuild;
		std::string filePath;
		bool debug = false;

		const std::vector<BWAPI::UnitType> protossBuildingTypesExceptPylon = { UnitTypes::Protoss_Arbiter_Tribunal, UnitTypes::Protoss_Assimilator, UnitTypes::Protoss_Citadel_of_Adun, UnitTypes::Protoss_Cybernetics_Core, UnitTypes::Protoss_Fleet_Beacon, UnitTypes::Protoss_Forge, UnitTypes::Protoss_Gateway, UnitTypes::Protoss_Nexus, UnitTypes::Protoss_Observatory, UnitTypes::Protoss_Photon_Cannon, UnitTypes::Protoss_Robotics_Facility, UnitTypes::Protoss_Shield_Battery, UnitTypes::Protoss_Stargate, UnitTypes::Protoss_Templar_Archives };

		inline UnitType randomProtossUnitType();

	public:
		void mutate(double chanceToRandomize, double chanceToCreate, double chanceToDelete);
		BWAPI::UnitType getNext();
		BuildOrder(std::string filePath);
		void write();

		void debugDraw();
		void enableDebugDraw(bool b = true) { debug = b; }
	};

}

inline UnitType ZerGreenAI::BuildOrder::randomProtossUnitType()
{
	return randFromVector(protossBuildingTypesExceptPylon);
}