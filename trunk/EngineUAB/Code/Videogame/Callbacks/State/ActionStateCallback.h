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
	void		StartAction				( void );
	bool		IsActionFinished		( void ) const			{ return m_CurrentTime >= m_TotalActionTime; }
	
	void		Update					( float _ElapsedTime )	{ m_CurrentTime += _ElapsedTime; }

private:
	// ------------- Mètodes -----------------------------------
	float		GetRandomAnimationTime	( void );
	void		SetTimeRange			( float _MinInterval, float _MaxInterval )		{ m_MinTime = _MinInterval; m_MaxTime = _MaxInterval; }

	// ------------- Members -----------------------------------
	float		m_MinTime;
	float		m_MaxTime;

	float		m_TotalActionTime;
	float		m_CurrentTime;
};

#endif _ACTION_STATE_CALLBACK_H_