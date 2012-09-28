#pragma once

#ifndef _DEER_IDLE_STATE_H_
#define _DEER_IDLE_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
// ---------------------------

class CDeerIdleState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerIdleState	( CCharacter* );
					CDeerIdleState	( CCharacter*, const std::string &_Name );
	virtual			~CDeerIdleState	( void );

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
	CDeer				  * m_pDeer;
	CActionStateCallback	m_ActionStateCallback;
	bool					m_AlreadyDetected;
	bool					m_AlreadyChased;
};


#endif _DEER_IDLE_STATE_H_