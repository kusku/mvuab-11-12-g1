#pragma once

#ifndef _PLAYER_HIT_STATE_H_
#define _PLAYER_HIT_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;
class CAnimationCallback;

class CPlayerHitState : public CState< CCharacter >
{
public:
	CPlayerHitState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerHitState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	CAnimationCallback	*m_pCallback;
};

#endif //_PLAYER_HIT_STATE_H_