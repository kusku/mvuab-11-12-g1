#pragma once

#ifndef _PLAYER_DEATH_ANIMATION_STATE_H_
#define _PLAYER_DEATH_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"

#include "Characters\Character.h"

// --- Foward Declarations ---
class CPlayer;
// ---------------------------

class CPlayerAnimationDeathState : public CState<CCharacter> 
{
public:
	// ------------- Constructors i Destructors --------------
					CPlayerAnimationDeathState	( CCharacter* );
					CPlayerAnimationDeathState	( CCharacter*, const std::string &_Name );
	virtual			~CPlayerAnimationDeathState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------

	//----Properties ------------------------------------------
	inline CPlayer * GetCharacter( void )		{ return m_pPlayer; }

	//----Members ---------------------------------------------
private:
	CPlayer *		m_pPlayer;

};


#endif _PLAYER_DEATH_ANIMATION_STATE_H_