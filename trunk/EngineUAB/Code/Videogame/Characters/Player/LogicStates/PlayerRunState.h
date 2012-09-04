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
	CPlayerRunState( const std::string &_Name );
	~CPlayerRunState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	float	m_fRotationVelocity;
};

#endif //_PLAYER_RUN_STATE_H_