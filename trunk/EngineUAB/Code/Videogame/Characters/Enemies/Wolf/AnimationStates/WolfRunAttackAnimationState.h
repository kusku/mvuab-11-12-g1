#pragma once

#ifndef _RABBIT_RUN_ATTACK_ANIMATION_STATE_H_
#define _RABBIT_RUN_ATTACK_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfRunAttackAnimationState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CWolfRunAttackAnimationState	( void );
					CWolfRunAttackAnimationState	( const std::string &_Name );
	virtual			~CWolfRunAttackAnimationState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter env�a un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	
	//----Properties ------------------------------------------
	inline CWolf * GetCharacter( void )		{ return m_pRabbit; }

	//----Members ---------------------------------------------
private:
	CWolf *		m_pRabbit;

};


#endif _RABBIT_RUN_ATTACK_ANIMATION_STATE_H_