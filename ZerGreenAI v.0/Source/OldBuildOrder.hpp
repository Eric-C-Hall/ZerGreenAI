#pragma once

#include <queue>

struct ProducerEmulate
{
	UnitType type;
	int id;
	int frames;
};

struct ProducerEmulateHash
{
	size_t operator()(ProducerEmulate a)
	{
		return a.id;
	}
};

bool operator==(const ProducerEmulate &a, const ProducerEmulate &b);

typedef std::unordered_set<ProducerEmulate, ProducerEmulateHash> ProducerEmulateSet;

struct BOResources
{
	double Minerals;
	int MineralHarvesters;
	double Vespene;
	int VespeneHarvesters;
	int SupplyCurrent;
	int SupplyTotal;
	ProducerEmulateSet Producers;
};

// minerals/vespene from one harvester per frame
double mineralRatePerHarvester();
double vespeneRatePerHarvester();

int rateEconomy();
int rateTech();
int rateMilitary();

enum TempFocus
{
	Economy = 0,

	Tech = 
};

class BuildPlans
{
	inline BOResources getCurrentResources();

public:
	std::queue<UnitType> contracts;
	std::queue<UnitType> planContracts();
};
