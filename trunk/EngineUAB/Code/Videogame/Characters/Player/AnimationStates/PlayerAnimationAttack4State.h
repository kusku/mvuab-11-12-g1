#pragma once

#ifndef _PLAYER_ANIMATION_ATTACK_4_STATE_H_
#define _PLAYER_ANIMATION_ATTACK_4_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

class CPlayerAnimationAttack4State : public CState< CCharacter >
{
public:
	CPlayerAnimationAttack4State( CCharacter* _pCharacter,const std::string &_Name );
	~CPlayerAnimationAttack4State();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );
};

#endif //_PLAYER_ANIMATION_ATTACK_4_STATE_H_