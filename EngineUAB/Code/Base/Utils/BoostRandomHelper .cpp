
#include "BoostRandomHelper.h"
#include "boost\random\uniform_int_distribution.hpp"
#include "boost\random\uniform_real_distribution.hpp"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

/********************************
*		Variable Def			*
*********************************/
mt11213b BoostRandomHelper::m_RGen = mt11213b();

/********************************
*		Function Def			*
*********************************/
float BoostRandomHelper::GetNextFloat()
{
	uniform_real_distribution<float> disRand;

	return disRand(m_RGen);
}

double BoostRandomHelper::GetNextDouble()
{
	uniform_real_distribution<double> disRand;

	return disRand(m_RGen);
}

float BoostRandomHelper::GetFloat(float max)
{
	uniform_real_distribution<float> disRand(0, max);

	return disRand(m_RGen);
}

float BoostRandomHelper::GetFloat(float min, float max)
{
	uniform_real_distribution<float> disRand(min, max);

	return disRand(m_RGen);
}

int32_t BoostRandomHelper::GetInt(int max)
{
	uniform_int_distribution<int> disRand(0, max);

	return disRand(m_RGen);
}

int32_t BoostRandomHelper::GetInt(int min, int max)
{
	uniform_int_distribution<int> disRand(min, max);

	return disRand(m_RGen);
}
