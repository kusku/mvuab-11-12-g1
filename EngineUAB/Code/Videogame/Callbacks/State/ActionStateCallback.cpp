#include "ActionStateCallback.h"
#include "Utils\BoostRandomHelper.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

CActionStateCallback::CActionStateCallback( float _MinValue, float _MaxValue )
	: m_MinTime			(_MinValue)
	, m_MaxTime			(_MaxValue)
	, m_CurrentTime		(_MinValue)
	, m_ActionStarted	(false)
{
}

CActionStateCallback::CActionStateCallback( float _MinValue, float _MinIntervalMaxValue, float _MaxIntervalMaxValue )
	: m_MinTime			(_MinValue)
	, m_MaxTime			(0.f)
	, m_CurrentTime		(_MinValue)
	, m_ActionStarted	(false)
{
	m_MaxTime = GetRandomAnimationTime(_MinIntervalMaxValue, _MaxIntervalMaxValue);
}

CActionStateCallback::~CActionStateCallback( void )
{
}

void CActionStateCallback::InitAction( void )														
{ 
	m_ActionStarted = false; 
}

void CActionStateCallback::InitAction( float _MinValue, float _MaxValue )		
{ 
	m_ActionStarted = false; 
	m_MinTime = _MinValue; 
	m_MaxTime = _MaxValue; 
	m_CurrentTime = _MinValue; 
}

void CActionStateCallback::InitAction( float _MinValue, float _MinIntervalMaxValue, float _MaxIntervalMaxValue )		
{ 
	m_ActionStarted = false; 
	m_MinTime = _MinValue; 
	m_MaxTime = GetRandomAnimationTime(_MinIntervalMaxValue, _MaxIntervalMaxValue); 
	m_CurrentTime = _MinValue; 
}


void CActionStateCallback::StartAction( void )
{
	m_CurrentTime = m_MinTime;
	m_ActionStarted = true;
}

bool CActionStateCallback::IsActionFinished( void )			
{ 
	if ( m_CurrentTime >= m_MaxTime )
	{
		m_ActionStarted = false;
		return true;
	}
	else
	{
		return false;
	}
}

float CActionStateCallback::GetRandomAnimationTime( float _MinInterval, float _MaxInterval )
{
	return BoostRandomHelper::GetFloat(_MinInterval, _MaxInterval);
}