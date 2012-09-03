#pragma once

#ifndef _RABBIT_PREPARED_TO_ATTACK_STATE_H_
#define _RABBIT_PREPARED_TO_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitPreparedToAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitPreparedToAttackState	( void );
					CRabbitPreparedToAttackState	( const std::string &_Name );
	virtual			~CRabbitPreparedToAttackState	( void );

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
	CRabbit					  * m_pRabbit;

};


#endif _RABBIT_PREPARED_TO_ATTACK_STATE_H_