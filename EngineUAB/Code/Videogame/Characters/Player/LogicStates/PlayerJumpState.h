#pragma once

#ifndef _PLAYER_JUMP_STATE_H_
#define _PLAYER_JUMP_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Math\Vector3.h"
#include <string>

struct STelegram;
class CAnimationCallback;

class CPlayerJumpState : public CState< CCharacter >
{
public:
	CPlayerJumpState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerJumpState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	CAnimationCallback	*m_pCallback;

	float				m_fJumpYaw;
	float				m_fTime;
	Vect3f				m_PrevPosition;
};

#endif //_PLAYER_JUMP_STATE_H_