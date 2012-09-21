#pragma once

#ifndef _DEER_STILL_ATTACK_STATE_H_
#define _DEER_STILL_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
//#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
class CActionStateCallback;
// ---------------------------

class CDeerStillAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
						CDeerStillAttackState	( CCharacter* );
						CDeerStillAttackState	( CCharacter*, const std::string &_Name );
	virtual				~CDeerStillAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void		Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void		OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void		OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool		OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	//svoid				SetParticlePosition		( CCharacter* _pCharacter );
	//void				SetParticlePosition		( CCharacter* _pCharacter, const std::string &ParticlesName, const std::string &Bone = "", const Vect3f &_Position = Vect3f(0,0,0));
	void				UpdateParticlesPositions( CCharacter* _pCharacter );
	
	// Para tema de partículas de impacto
	void				GenerateImpact			( CCharacter* _pCharacter, bool _FirstImpact = true );
	void				UpdateImpact			( CCharacter* _pCharacter );


	//----Properties ------------------------------------------
	//CParticleEmitter*	//GetParticleEmitter( const std::string & _EmitterName ) const { return CORE->//GetParticleEmitterManager()->GetResource(_EmitterName); }

	//----Members ---------------------------------------------
private:
	CDeer					  * m_pDeer;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback	  *	m_pActionStateCallback;
	
	bool						m_SoundPlayed1;			// Indica si el sonido ya se inició
	bool						m_SoundPlayed2;			// Indica si el sonido ya se inició
	bool						m_FirstHitDone;			// Indica si el primer golpe se realizó
	bool						m_SecondHitDone;		// Indica si el segundo golpe se realizó
	bool						m_FirstParticlesHitDone;		// Indica si el primer golpe se realizó y se mostró ya las partículas
	bool						m_SecondParticlesHitDone;		// Indica si el segundo golpe se realizó y se mostró ya las partículas

	bool						m_FirstHitReached;		// Indica si he golpeado el player en el primer golpeo
	bool						m_SecondHitReached;		// Indica si he golpeado el player en el segundo golpeo
};


#endif _DEER_STILL_ATTACK_STATE_H_