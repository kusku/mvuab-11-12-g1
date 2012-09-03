#pragma once

#ifndef _RABBIT_RUN_ANIMATION_STATE_H_
#define _RABBIT_RUN_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

class CRabbitRunAnimationState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CRabbitRunAnimationState	( void );
					CRabbitRunAnimationState	( const std::string &_Name );
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