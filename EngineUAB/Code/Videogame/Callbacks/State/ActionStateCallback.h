#pragma once

#ifndef _ACTION_STATE_CALLBACK_H_
#define _ACTION_STATE_CALLBACK_H_

class CActionStateCallback
{
public:
	// ------------- Constructors i Destructors ----------------
				CActionStateCallback	( float _MinInterval, float _MaxInterval );
				~CActionStateCallback	( void );

	// ------------- Mètodes Principals ------------------------
	void		InitAction				( void )				{ m_ActionStarted = false; }
	void		StartAction				( void );
	bool		IsActionFinished		( void );
	bool		IsActionStarted			( void ) const			{ return m_ActionStarted; }
	
	void		Update					( float _ElapsedTime )	{ m_CurrentTime += _ElapsedTime; }

	// ------------- Mètodes -----------------------------------
	void		SetTimeRange			( float _MinInterval, float _MaxInterval )		{ m_MinTime = _MinInterval; m_MaxTime = _MaxInterval; }

private:
	float		GetRandomAnimationTime	( void );
	
	// ------------- Members -----------------------------------
	float		m_MinTime;
	float		m_MaxTime;

	float		m_TotalActionTime;
	float		m_CurrentTime;
	bool		m_ActionStarted;
};

#endif _ACTION_STATE_CALLBACK_H_