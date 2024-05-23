#include "stdafx.h"

int Randomizer::GetNextInt()
{
	return Randomizer::GetNextInt(INT_MIN, INT_MAX);
}

int Randomizer::GetNextInt(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distVal(min, max);

	return distVal(rng);
}
