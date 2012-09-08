#pragma once

#ifndef _DEER_RUN_ATTACK_STATE_H_
#define _DEER_RUN_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
// ---------------------------

class CDeerRunAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerRunAttackState	( void );
					CDeerRunAttackState	( const std::string &_Name );
	virtual			~CDeerRunAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------

	//----Members ---------------------------------------------
private:
	CDeer				*	m_pDeer;
	CAnimationCallback	*	m_pAnimationCallback;

	float					m_OldMaxSpeed;			// Permite almacenar la vieja velocidad para posteriormente recuperarla
	float					m_OldMass;				// Permite almacenar la vieja masa para posteriormente recuperarla
	float					m_CurrentDistance;		// Permite alamacenar la distancia para saber si luego nos pasamos

	float					m_CurrentDuration;
	float					m_AnimationDuration;

};


#endif _DEER_RUN_ATTACK_STATE_H_