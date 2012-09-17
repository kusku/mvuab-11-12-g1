#pragma once

#ifndef _RABBIT_STILL_ATTACK_STATE_H_
#define _RABBIT_STILL_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CRabbit;
class CAnimationCallback;
class CParticleEmitter;
// ---------------------------

class CRabbitStillAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitStillAttackState	( void );
					CRabbitStillAttackState	( const std::string &_Name );
	virtual			~CRabbitStillAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			SetParticlePosition	( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CRabbit					  * m_pRabbit;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback		m_pActionStateCallback;

	bool						m_SoundPlayed1;			// Indica si el sonido ya se inició
	bool						m_SoundPlayed2;			// Indica si el sonido ya se inició

	// Tema Partículas
	CParticleEmitter	*m_pParticleEmitter;

};


#endif _RABBIT_STILL_ATTACK_STATE_H_