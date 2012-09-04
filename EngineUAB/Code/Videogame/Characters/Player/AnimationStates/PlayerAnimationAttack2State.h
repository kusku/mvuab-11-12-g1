#pragma once

#ifndef _PLAYER_ANIMATION_ATTACK_2_STATE_H_
#define _PLAYER_ANIMATION_ATTACK_2_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

class CPlayerAnimationAttack2State : public CState< CCharacter >
{
public:
	CPlayerAnimationAttack2State( const std::string &_Name );
	~CPlayerAnimationAttack2State();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );
};

#endif //_PLAYER_ANIMATION_ATTACK_2_STATE_H_