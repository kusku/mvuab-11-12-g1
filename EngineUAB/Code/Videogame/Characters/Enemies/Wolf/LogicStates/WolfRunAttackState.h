#pragma once

#ifndef _WOLF_RUN_ATTACK_STATE_H_
#define _WOLF_RUN_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CWolf;
class CAnimationCallback;
// ---------------------------

class CWolfRunAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfRunAttackState	( CCharacter* );
					CWolfRunAttackState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfRunAttackState( void );

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
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );
	void			StopImpact				( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CWolf					  * m_pWolf;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback	  	m_pActionStateCallback;

	float						m_InitialDistance;			// Permite alamacenar la distancia inicial para saber si luego nos pasamos
	Vect3f						m_PlayerInitialPosition;	// Permite saber la distancia de inicio de ataque del player. Así sabremos si hemos pasado de largo.
	Vect3f						m_FinalAttackPosition;		// Permite saber la posición final de toda la animación 

	float						m_AnimationDuration;
	
	bool						m_playerPushed;
	sDireccion					m_AditionalInfo;
};


#endif _WOLF_RUN_ATTACK_STATE_H_