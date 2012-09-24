#pragma once

#ifndef _RABBIT_RUN_ATTACK_STATE_H_
#define _RABBIT_RUN_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\StatesDefs.h"

// --- Foward Declarations ---
class CRabbit;
class CAnimationCallback;
// ---------------------------

class CRabbitRunAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitRunAttackState	( CCharacter* );
					CRabbitRunAttackState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitRunAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			PlayRandomSound	( void );
	
	// Para tema de partículas
	void			UpdateParticlesPositions( CCharacter* _pCharacter );
	
	// Para tema de partículas de impacto
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );
	void			StopImpact				( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CRabbit				*	m_pRabbit;
	CAnimationCallback	*	m_pAnimationCallback;
	CActionStateCallback	m_ActionStateCallback;

	//float					m_OldMaxSpeed;				// Permite almacenar la vieja velocidad para posteriormente recuperarla
	float					m_OldMass;					// Permite almacenar la vieja masa para posteriormente recuperarla
	float					m_CurrentDistance;			// Permite alamacenar la distancia para saber si luego nos pasamos
	float					m_InitialDistance;			// Permite alamacenar la distancia inicial para saber si luego nos pasamos
	Vect3f					m_PlayerInitialPosition;	// Permite saber la distancia de inicio de ataque del player. Así sabremos si hemos pasado de largo.
	Vect3f					m_FinalAttackPosition;		// Permite saber la posición final de toda la animación 

	float					m_AnimationDuration;
	
	bool					m_playerPushed;
	sDireccion				m_AditionalInfo;

	float					m_SoundDuration;			// Permite saber la duración del sonido
	bool					m_FirstParticlesHitDone;	// Indiaca que ya se ha lanzado las partículas de impacto
};


#endif _RABBIT_RUN_ATTACK_STATE_H_