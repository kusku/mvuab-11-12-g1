#include "ActionStateCallback.h"
#include "Utils\BoostRandomHelper.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

CActionStateCallback::CActionStateCallback( float _MinInterval, float _MaxInterval )
	: m_MinTime (_MinInterval)
	, m_MaxTime (_MaxInterval)
{
}


CActionStateCallback::~CActionStateCallback( void )
{
}

void CActionStateCallback::StartAction( void )
{
	m_TotalActionTime = GetRandomAnimationTime();
	m_CurrentTime = 0.f;
}

float CActionStateCallback::GetRandomAnimationTime( void )
{
	return BoostRandomHelper::GetFloat(m_MinTime, m_MaxTime);
}