#pragma once

#ifndef _PLAYER_DEFENSE_STATE_H_
#define _PLAYER_DEFENSE_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

class CPlayerDefenseState : public CState< CCharacter >
{
public:
	CPlayerDefenseState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerDefenseState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );
};

#endif //_PLAYER_DEFENSE_STATE_H_