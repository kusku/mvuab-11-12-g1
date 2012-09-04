#pragma once

#ifndef _PLAYER_ANIMATION_DEFENSE_STATE_H_
#define _PLAYER_ANIMATION_DEFENSE_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

class CPlayerAnimationDefenseState : public CState< CCharacter >
{
public:
	CPlayerAnimationDefenseState( const std::string &_Name );
	~CPlayerAnimationDefenseState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );
};

#endif //_PLAYER_ANIMATION_DEFENSE_STATE_H_