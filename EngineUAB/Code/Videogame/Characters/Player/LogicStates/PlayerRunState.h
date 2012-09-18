#pragma once

#ifndef _PLAYER_RUN_STATE_H_
#define _PLAYER_RUN_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

class CPlayerRunState : public CState< CCharacter >
{
public:
	CPlayerRunState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerRunState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	float	m_fRotationVelocity;
	float	m_fAccelerationStart;
	float	m_fAccelerationEnd;
	float	m_fMaxVelocityMovement;
	float	m_fCurrentVelocityMovement;

	bool	m_bStartState;
	bool	m_bEndState;
	Vect3f  m_LastDirection;
};

#endif //_PLAYER_RUN_STATE_H_