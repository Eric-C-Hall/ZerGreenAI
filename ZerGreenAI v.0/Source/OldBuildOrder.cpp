#include "OldBuildOrder.h"
#include "GlobalHarvesting.h"
#include "OldProduction.h"
#include "Namespaces.h"

double mineralRatePerHarvester()
{
	return 0.04533333333; // experimental rate
}

double vespeneRatePerHarvester()
{
	return 0.07066666666; // experimental rate
}

bool operator==(const ProducerEmulate &a, const ProducerEmulate &b)
{
	return a.id == b.id;
}

ProducerEmulateSet producerEmulateFromUnitset(Unitset uset)
{
	ProducerEmulateSet output;
	for (auto const &u : uset)
	{
		ProducerEmulate temp;
		temp.frames = u->getRemainingTrainTime();
		temp.type = u->getType();
		temp.id = u->getID();
		output.insert(temp);
	}
	return output;
}

inline BOResources BuildPlans::getCurrentResources()
{
	BOResources output;
	output.Minerals = Broodwar->self()->minerals();
	output.MineralHarvesters = getGlobalHarvester()->numMineralHarvesters();
	output.Vespene = Broodwar->self()->gas();
	output.VespeneHarvesters = getGlobalHarvester()->numGasHarvesters();
	output.Producers = producerEmulateFromUnitset(getProductionManager()->getProductionBuildings());
	output.SupplyCurrent = Broodwar->self()->supplyUsed();
	output.SupplyTotal = Broodwar->self()->supplyTotal();

	return output;
}

int rateEconomy()
{

}

int rateTech()
{

}

int rateMilitary()
{

}

UnitType addNextToContract(BOResources &resources) // modifies resources
{
	
	int leastFrames;
	ProducerEmulate leastFramesProducer;
	for (auto const &p : resources.Producers)
	{
		if (p.frames - 48 < 0)
		{

		}
	}
	return UnitTypes::Zerg_Larva;
}

std::queue<UnitType> BuildPlans::planContracts()
{
	std::queue<UnitType> returnContract;
	BOResources planTimeResources = getCurrentResources();

	for (int i = 0; i < 50; i++)
	{
		returnContract.push(addNextToContract(planTimeResources));
	}

	return returnContract;
}