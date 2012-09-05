#pragma once

#ifndef _RABBIT_HIT_ANIMATION_STATE_H_
#define _RABBIT_HIT_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declaractions ---
class CWolf;
// ----------------------------

class CWolfHitAnimationState : public CState<CCharacter> 
{
public:
	// ------------- Constructors i Destructors --------------
					CWolfHitAnimationState	( void );
					CWolfHitAnimationState	( const std::string &_Name );
	virtual			~CWolfHitAnimationState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------

	//----Properties ------------------------------------------
	inline CWolf * GetCharacter( void )		{ return m_pRabbit; }

	//----Members ---------------------------------------------
private:
	CWolf *		m_pRabbit;

};


#endif _RABBIT_IDLE_ANIMATION_STATE_H_