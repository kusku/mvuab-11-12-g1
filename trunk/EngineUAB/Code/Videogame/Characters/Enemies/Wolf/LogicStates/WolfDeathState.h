#pragma once

#ifndef _WOLF_DEATH_STATE_H_
#define _WOLF_DEATH_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
class CAnimationCallback;
// ---------------------------

class CWolfDeathState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfDeathState	( CCharacter* );
					CWolfDeathState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfDeathState	( void );

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
	CAnimationCallback		  *	m_pAnimationCallback;
};
#endif _WOLF_DEATH_STATE_H_