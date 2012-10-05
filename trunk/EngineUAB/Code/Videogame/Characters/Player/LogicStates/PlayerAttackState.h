#pragma once

#ifndef _PLAYER_ATTACK_STATE_H_
#define _PLAYER_ATTACK_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

class CGameProcess;
class CActionToInput;
class CAnimationCallback;
class CParticleEmitterInstance;
struct STelegram;

class CPlayerAttackState : public CState< CCharacter >
{
public:
	CPlayerAttackState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerAttackState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

private:
	bool	CalculateAngleMovement( CCharacter *_pCharacter, float &_fAngle );
	void	SetParticlePosition( CCharacter* _pCharacter );

	void			SetParticlePosition		( CCharacter* _pCharacter, const std::string &ParticlesName, const std::string &Bone = "", const Vect3f &_Position = Vect3f(0,0,0));
	void			UpdateParticlesPositions( CCharacter* _pCharacter );

	// Para tema de partículas de impacto
	void			GenerateImpact			( void );
	void			UpdateImpact			( CCharacter* _pCharacter );

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

#endif //_PLAYER_ATTACK_STATE_H_