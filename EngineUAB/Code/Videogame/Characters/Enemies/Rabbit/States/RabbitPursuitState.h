#pragma once

#ifndef _RABBIT_PURSUIT_STATE_H_
#define _RABBIT_PURSUIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CRabbitPursuitState		( void );
					CRabbitPursuitState		( const std::string &_Name );
	virtual			~CRabbitPursuitState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter env�a un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------


	//----Members ---------------------------------------------
private:
	CRabbit					  * m_pRabbit;

};


#endif _RABBIT_PURSUIT_STATE_H_