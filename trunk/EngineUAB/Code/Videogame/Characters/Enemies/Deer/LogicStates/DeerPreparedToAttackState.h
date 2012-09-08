#pragma once

#ifndef _DEER_PREPARED_TO_ATTACK_STATE_H_
#define _DEER_PREPARED_TO_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CDeer;
// ---------------------------

class CDeerPreparedToAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerPreparedToAttackState	( void );
					CDeerPreparedToAttackState	( const std::string &_Name );
	virtual			~CDeerPreparedToAttackState	( void );

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
	CDeer					  * m_pDeer;

};


#endif _DEER_PREPARED_TO_ATTACK_STATE_H_