#pragma once

#ifndef _DEER_RUN_ATTACK_STATE_H_
#define _DEER_RUN_ATTACK_STATE_H_


#include <string>
#include "Math\Vector3.h"
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
// ---------------------------

class CDeerRunAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerRunAttackState	( CCharacter* );
					CDeerRunAttackState	( CCharacter*, const std::string &_Name );
	virtual			~CDeerRunAttackState( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	
	// Para tema de partículas de impacto
	void				GenerateImpact			( CCharacter* _pCharacter );
	void				UpdateImpact			( CCharacter* _pCharacter );
	void				StopImpact				( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CDeer				*	m_pDeer;
	CAnimationCallback	*	m_pAnimationCallback;

	float					m_OldMaxSpeed;				// Permite almacenar la vieja velocidad para posteriormente recuperarla
	float					m_OldMass;					// Permite almacenar la vieja masa para posteriormente recuperarla
	float					m_InitialDistance;			// Permite alamacenar la distancia inicial para saber si luego nos pasamos
	Vect3f					m_PlayerInitialPosition;	// Permite saber la distancia de inicio de ataque del player. Así sabremos si hemos pasado de largo.
	Vect3f					m_FinalAttackPosition;		// Permite saber la posición final de toda la animación 

	float					m_AnimationDuration;
	
	bool					m_playerPushed;
	sDireccion				m_AditionalInfo;

};


#endif _DEER_RUN_ATTACK_STATE_H_