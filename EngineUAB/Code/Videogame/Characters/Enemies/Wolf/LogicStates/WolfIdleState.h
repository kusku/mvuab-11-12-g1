#pragma once

#ifndef _WOLF_IDLE_STATE_H_
#define _WOLF_IDLE_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfIdleState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfIdleState	( CCharacter* );
					CWolfIdleState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfIdleState	( void );

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

	//----Members ---------------------------------------------
private:
	CWolf				  * m_pWolf;
	CActionStateCallback	m_ActionStateCallback;
	bool					m_AlreadyDetected;
	bool					m_AlreadyChased;
	bool					m_IdleWarningSounds;
};


#endif _WOLF_IDLE_STATE_H_