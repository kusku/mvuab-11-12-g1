#pragma once

#ifndef _DEER_ATTACK_STATE_H_
#define _DEER_ATTACK_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CDeer;
// ---------------------------

class CDeerAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerAttackState	( void );
					CDeerAttackState	( const std::string &_Name );
	virtual			~CDeerAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	std::string		GetRandomAttackName	(void);

	//----Members ---------------------------------------------
private:
	CActionStateCallback		m_ActionTime;
	CDeer					  * m_pDeer;
	
};


#endif _DEER_ATTACK_STATE_H_