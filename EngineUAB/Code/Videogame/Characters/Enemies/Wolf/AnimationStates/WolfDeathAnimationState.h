#pragma once

#ifndef _WOLF_DEATH_ANIMATION_STATE_H_
#define _WOLF_DEATH_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"

#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfDeathAnimationState : public CState<CCharacter> 
{
public:
	// ------------- Constructors i Destructors --------------
					CWolfDeathAnimationState	( CCharacter* );
					CWolfDeathAnimationState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CWolfDeathAnimationState	( void );

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
	inline CWolf * GetCharacter( void )		{ return m_pWolf; }

	//----Members ---------------------------------------------
private:
	CWolf *		m_pWolf;

};


#endif _WOLF_IDLE_ANIMATION_STATE_H_