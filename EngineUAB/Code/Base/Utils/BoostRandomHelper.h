
#pragma once

#ifndef _BOOST_RANDOM_HELPER_H
#define _BOOST_RANDOM_HELPER_H

#include "Base.h"
#include "boost\random\mersenne_twister.hpp"

using namespace boost;
using namespace random;

struct BoostRandomHelper
{
private:
	static mt11213b		m_RGen;

public:
	static double		GetNextDouble			();
	static float		GetNextFloat			();
	static float		GetFloat				(float max);
	static float		GetFloat				(float min, float max);
	static int32		GetInt					(int max);
};

#endif