#pragma once

#ifndef _RABBIT_RUN_ANIMATION_STATE_H_
#define _RABBIT_RUN_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitRunAnimationState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CRabbitRunAnimationState	( CCharacter* );
					CRabbitRunAnimationState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitRunAnimationState	( void );

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
	inline CRabbit * GetCharacter( void )		{ return m_pRabbit; }

	//----Members ---------------------------------------------
private:
	CRabbit *		m_pRabbit;

};


#endif _RABBIT_RUN_ANIMATION_STATE_H_