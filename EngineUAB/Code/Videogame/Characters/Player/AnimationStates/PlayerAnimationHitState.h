#pragma once

#ifndef _PLAYER_ANIMATION_HIT_STATE_H_
#define _PLAYER_ANIMATION_HIT_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

class CPlayerAnimationHitState : public CState< CCharacter >
{
public:
	CPlayerAnimationHitState( CCharacter* _pCharacter, const std::string &_Name );
	~CPlayerAnimationHitState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );
};

#endif //_PLAYER_ANIMATION_HIT_STATE_H_