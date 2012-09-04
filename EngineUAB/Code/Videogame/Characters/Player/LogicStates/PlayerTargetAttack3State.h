#pragma once

#ifndef _PLAYER_TARGET_ATTACK_3_STATE_H_
#define _PLAYER_TARGET_ATTACK_3_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;
class CAnimationCallback;

class CPlayerTargetAttack3State : public CState< CCharacter >
{
public:
	CPlayerTargetAttack3State( const std::string &_Name );
	~CPlayerTargetAttack3State();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	CAnimationCallback	*m_pCallback;
};

#endif //_PLAYER_TARGET_ATTACK_3_STATE_H_