#pragma once

#ifndef _DEER_STILL_ATTACK_STATE_H_
#define _DEER_STILL_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
class CParticleEmitter;
// ---------------------------

class CDeerStillAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerStillAttackState	( void );
					CDeerStillAttackState	( const std::string &_Name );
	virtual			~CDeerStillAttackState	( void );

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
	CDeer					  * m_pDeer;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback		m_pActionStateCallback;
	CParticleEmitter		  * m_pParticleEmitter1;
	CParticleEmitter		  * m_pParticleEmitter2;
	CParticleEmitter		  * m_pParticleEmitter3;
	CParticleEmitter		  * m_pParticleEmitter4;

	bool						m_SoundPlayed1;			// Indica si el sonido ya se inició
	bool						m_SoundPlayed2;			// Indica si el sonido ya se inició
	bool						m_FirstHitDone;			// Indica si el primer golpe se realizó
	bool						m_SecondHitDone;		// Indica si el segundo golpe se realizó
	bool						m_FirstHitReached;		// Indica si he golpeado el player en el primer golpeo
	bool						m_SecondHitReached;		// Indica si he golpeado el player en el segundo golpeo
};


#endif _DEER_STILL_ATTACK_STATE_H_