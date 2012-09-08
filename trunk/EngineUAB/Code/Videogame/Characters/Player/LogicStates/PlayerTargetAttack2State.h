#pragma once

#ifndef _PLAYER_TARGET_ATTACK_2_STATE_H_
#define _PLAYER_TARGET_ATTACK_2_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;
class CAnimationCallback;

class CPlayerTargetAttack2State : public CState< CCharacter >
{
public:
	CPlayerTargetAttack2State( const std::string &_Name );
	~CPlayerTargetAttack2State();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	CAnimationCallback	*m_pCallback;

	bool				m_bFirstUpdate;
	float				m_fMaxVelocityMovement;
	float				m_fCurrentVelocityMovement;
	float				m_fAccelerationMovement;
};

#endif //_PLAYER_TARGET_ATTACK_2_STATE_H_