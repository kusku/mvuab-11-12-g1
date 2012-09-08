#pragma once

#ifndef _PLAYER_ATTACK_2_STATE_H_
#define _PLAYER_ATTACK_2_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;
class CAnimationCallback;

class CPlayerAttack2State : public CState< CCharacter >
{
public:
	CPlayerAttack2State( const std::string &_Name );
	~CPlayerAttack2State();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	CAnimationCallback	*m_pCallback;

	bool				m_bFirstUpdate;
};

#endif //_PLAYER_ATTACK_2_STATE_H_