#pragma once

#ifndef _DEER_TIRED_STATE_H_
#define _DEER_TIRED_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CDeer;
// ---------------------------

class CDeerTiredState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerTiredState	( void );
					CDeerTiredState	( const std::string &_Name );
	virtual			~CDeerTiredState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter env�a un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	//std::string		GetRandomAttackName	( void );

	//----Members ---------------------------------------------
private:
	CActionStateCallback		m_ActionTime;
	CDeer					  * m_pDeer;
	
	std::string					m_ActiveActionState;		// Me indica qu� acci�n o estado vamos a ejecutar despu�s de hacer un random de todas ellas
	
};


#endif _DEER_TIRED_STATE_H_