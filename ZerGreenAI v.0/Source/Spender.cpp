#include "stdafx.h"

#include "Spender.hpp"
#include "ZerGreenAI.hpp"
#include "Production.hpp"
#include "ConstructionBrain.hpp"
#include "Construction.hpp"
#include "ProductionBrain.hpp"
#include "ConstructionUtility.hpp"

void ZerGreenAI::Spender::onStart()
{
	//buildOrder.mutate(0.05,0.05,0.06);
}

void ZerGreenAI::Spender::onFrame()
{
	buildOrder.debugDraw();

	if (Broodwar->getFrameCount() % FRAMES_BETWEEN_SPENDING != 0)
		return;

	if (makingUnit)
	{
		if (ZerGreenAIObj::mainInstance->productionManager->produceType(makeUnitType))
		{
			makingUnit = false;
		}
		return;
	}

	if (makingBuilding)
	{
		if (ZerGreenAIObj::mainInstance->constructionManager->constructBuilding(makeBuildingType))
		{
			makingBuilding = false;
		}
		return;
	}

	if (!makingUnit && ZerGreenAIObj::mainInstance->productionManager->unassignedBuildingTypes().size() != 0)
	{
		UnitType currentType = ZerGreenAIObj::mainInstance->productionManager->unassignedBuildingTypes().front();
		
		switch (currentType)
		{
		case UnitTypes::Protoss_Gateway:
			makeUnitType = ZerGreenAIObj::mainInstance->brainManager->gatewayBrain->choose();
			makingUnit = true;
			break;
		case UnitTypes::Protoss_Stargate:
			makeUnitType = ZerGreenAIObj::mainInstance->brainManager->stargateBrain->choose();
			makingUnit = true;
			break;
		case UnitTypes::Protoss_Robotics_Facility:
			makeUnitType = ZerGreenAIObj::mainInstance->brainManager->roboFacBrain->choose();
			makingUnit = true;
			break;
		default:
			assert(false);
			break;
		}

		if (makeUnitType == UnitTypes::None)
		{
			makingUnit = false;
			ZerGreenAIObj::mainInstance->productionManager->tieDown(currentType);
		}
		else
		{
			Broodwar << makeUnitType.c_str() << std::endl;
		}
		return;
	}

	//for (every upgrade building)
	//{
	//	choose what to do
	//
	//	if no money left then return
	//}
	
	if (!makingBuilding)
	{
		//makeBuildingType = ZerGreenAIObj::mainInstance->brainManager->constructionBrain->choose();
		makeBuildingType = buildOrder.getNext();
		if (isBuildingReasonableBuildChoice(makeBuildingType) && !ZerGreenAIObj::mainInstance->constructionManager->isMakingBuilding(makeBuildingType))
		{
			Broodwar << makeBuildingType.c_str() << std::endl;
			makingBuilding = true;
			return;
		}
	}
}

void ZerGreenAI::Spender::onSendText(std::string text)
{
	if (text == "buildOrder")
	{
		buildOrder.enableDebugDraw();
	}
}

void ZerGreenAI::Spender::onEnd(bool didWin)
{
	if (didWin)
	{
		buildOrder.write();
	}
}
