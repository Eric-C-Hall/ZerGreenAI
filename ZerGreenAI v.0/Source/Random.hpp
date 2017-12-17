#pragma once

#include <random>

inline bool trueWithProb(double probability)
{
	assert(probability >= 0 && probability <= 1);
	return probability > ((double)rand() / (double)RAND_MAX);
}

template <typename T>
inline T randFromVector(std::vector<T> v)
{
	return v[rand() % v.size()];
}