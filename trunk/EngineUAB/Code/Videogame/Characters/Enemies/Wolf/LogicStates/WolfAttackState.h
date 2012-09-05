#pragma once

#ifndef _RABBIT_ATTACK_STATE_H_
#define _RABBIT_ATTACK_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfAttackState	( void );
					CWolfAttackState	( const std::string &_Name );
	virtual			~CWolfAttackState	( void );

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
	CWolf					  * m_pRabbit;
	
};


#endif _RABBIT_ATTACK_STATE_H_