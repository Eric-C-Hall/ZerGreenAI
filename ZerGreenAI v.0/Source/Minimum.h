#pragma once
#include <unordered_set>
// Minimum takes function, set returns minimum

// A is the contained type
//template <class a2b, class a, class b>
//a minimum(std::unordered_set<a> set);



template <class a2b, class a, class b>
a minimum(std::unordered_set<a> set)
{
	bool minContains = false;
	a returnValue;
	b currentSmallest;
	a2b func;

	for (auto const &t : set)
	{
		b currentCheck = func(t);
		if (!minContains || currentCheck < currentSmallest)
		{
			returnValue = t;
			currentSmallest = currentCheck;
		}
	}
	if (minContains)
	{
		return returnValue;
	}
	else
	{
		throw;
	}
}