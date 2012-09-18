#pragma once

#ifndef _RABBIT_IDLE2_ANIMATION_STATE_H_
#define _RABBIT_IDLE2_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitIdle2AnimationState : public CState<CCharacter> 
{
public:
	// ------------- Constructors i Destructors --------------
					CRabbitIdle2AnimationState	( CCharacter* );
					CRabbitIdle2AnimationState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitIdle2AnimationState	( void );

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
	inline CRabbit * GetCharacter( void )		{ return m_pRabbit; }

	//----Members ---------------------------------------------
private:
	CRabbit *		m_pRabbit;

};


#endif _RABBIT_IDLE_ANIMATION_STATE_H_