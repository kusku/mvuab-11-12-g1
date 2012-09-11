#pragma once

#ifndef _WOLF_TIRED_STATE_H_
#define _WOLF_TIRED_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfTiredState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfTiredState	( void );
					CWolfTiredState	( const std::string &_Name );
	virtual			~CWolfTiredState	( void );

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
	CActionStateCallback		m_ActionTime;
	CWolf					  * m_pWolf;
	
	std::string					m_ActiveActionState;		// Me indica qué acción o estado vamos a ejecutar después de hacer un random de todas ellas
	
};


#endif _WOLF_TIRED_STATE_H_