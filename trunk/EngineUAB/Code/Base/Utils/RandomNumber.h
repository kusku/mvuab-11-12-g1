#pragma once

#ifndef _RANDOM_NUMBER_H
#define _RANDOM_NUMBER_H

#include "Utils\Types.h"
#include <random>
#include <math.h>

template <class T>
class CRandomNumber
{
public:
	CRandomNumber()
	{
	}

	inline void SetSeed (uint32 seed)
	{
		srand(seed);
	}

	inline T operator () ( T max, T min )
	{
		return static_cast<T>( rand() % static_cast<int>((abs(max - min) + 1)) + min );
	}
};

#endif