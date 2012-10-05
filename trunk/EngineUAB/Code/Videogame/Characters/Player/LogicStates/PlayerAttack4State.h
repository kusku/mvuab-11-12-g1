#pragma once

#ifndef _PLAYER_ATTACK_4_STATE_H_
#define _PLAYER_ATTACK_4_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

class CGameProcess;
class CActionToInput;
class CAnimationCallback;
class CParticleEmitterInstance;
struct STelegram;

class CPlayerAttack4State : public CState< CCharacter >
{
public:
	CPlayerAttack4State( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerAttack4State();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	bool	CalculateAngleMovement( CCharacter *_pCharacter, float &_fAngle );
	void	SetParticlePosition( CCharacter* _pCharacter );

private:
	CAnimationCallback			*m_pCallback;
	CParticleEmitterInstance	*m_pParticleEmitter;
	CGameProcess				*m_pProcess;
	CActionToInput				*m_pInput;

	bool						m_bFirstUpdate;
	float						m_fMaxVelocityMovement;
	float						m_fCurrentVelocityMovement;
	float						m_fAccelerationMovement;
	float						m_fAttackDistance;
	float						m_fDetectionDistance;
	float						m_fAttackAngle;
	float						m_fDetectionAngle;
	float						m_fAttackYaw;
};

#endif //_PLAYER_ATTACK_4_STATE_H_