#pragma once

#ifndef _RABBIT_HIT_STATE_H_
#define _RABBIT_HIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CRabbit;
class CAnimationCallback;
// ---------------------------

class CRabbitHitState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitHitState	( CCharacter* );
					CRabbitHitState	( CCharacter*, const std::string &_Name );
	virtual			~CRabbitHitState( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			PlayRandomSound( void );
	void			SetParticlePosition		( CCharacter* _pCharacter, const std::string &ParticlesName, const std::string &Bone = "", const Vect3f &_Position = Vect3f(0,0,0));
	void			UpdateParticlesPositions( CCharacter* _pCharacter );


	//----Members ---------------------------------------------
private:
	CRabbit					  * m_pRabbit;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback		m_pActionState;
	float						m_SoundDuration;

	bool						m_IsCommingFromTired;
	float						m_RecoverMinTiredTime;
	float						m_RecoverMaxTiredTime;
	
};
#endif _RABBIT_HIT_STATE_H_