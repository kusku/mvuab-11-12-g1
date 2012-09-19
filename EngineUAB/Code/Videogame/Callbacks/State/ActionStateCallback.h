#pragma once

#ifndef _ACTION_STATE_CALLBACK_H_
#define _ACTION_STATE_CALLBACK_H_

class CActionStateCallback
{
public:
	// ------------- Constructors i Destructors ----------------
				CActionStateCallback	( float _MinValue, float _MaxValue );
				CActionStateCallback	( float _MinValue, float _MinIntervalMaxValue, float _MaxIntervalMaxValue );
				~CActionStateCallback	( void );

	// ------------- Mètodes Principals ------------------------
	void		InitAction				( void );
	void		InitAction				( float _MinValue, float _MaxValue);
	void		InitAction				( float _MinValue, float _MinInterval, float _MaxInterval );
	
	void		StartAction				( void );
	
	bool		IsActionFinished		( void );
	bool		IsActionStarted			( void ) const													{ return m_ActionStarted; }
	bool		IsActionInTime			( float _TimeToCheck )											{ return m_CurrentTime >= _TimeToCheck; }

	void		Update					( float _ElapsedTime )											{ m_CurrentTime += _ElapsedTime; }

	// ------------- Mètodes -----------------------------------
	void		SetTimeRange			( float _MinValue, float _MaxValue )							{ m_MinTime = _MinValue; m_MaxTime = _MaxValue; }

	// ------------- Properties---------------------------------
	

private:
	float		GetRandomAnimationTime	( float _MinInterval, float _MaxInterval );
	
	// ------------- Members -----------------------------------
	float		m_MinTime;
	float		m_MaxTime;

	float		m_CurrentTime;
	bool		m_ActionStarted;
};

#endif _ACTION_STATE_CALLBACK_H_