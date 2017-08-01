

// In GlobalHarvesting, nearbyAvailableHarvester
// More generic but probably slower and also checks bases with no workers

/*	

Position manager2DistanceCurrentPoint;
struct Manager2Distance
{
double operator()(LocalHarvestManager * mgr)
{
return mgr->getPosition().getApproxDistance(manager2DistanceCurrentPoint);
}
};

manager2DistanceCurrentPoint = p;
try
	{
		return minimum<Manager2Distance, LocalHarvestManager *, double>(childManagers)->nearbyAvailableHarvester(p);
	}
	catch (const std::exception & e)
	{
		Broodwar << "EXCEPTION in nearbyAvailableHarvester: " << e.what() << std::endl;
		return nullptr;
		}*/