#pragma once

#ifndef _Wolf_PURSUIT_STATE_H_
#define _Wolf_PURSUIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CWolfPursuitState		( void );
					CWolfPursuitState		( const std::string &_Name );
	virtual			~CWolfPursuitState	( void );

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
	CWolf					  * m_pWolf;

};


#endif _Wolf_PURSUIT_STATE_H_