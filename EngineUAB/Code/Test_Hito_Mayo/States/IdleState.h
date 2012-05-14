#pragma once

#ifndef __CLASS_IDLE_STATE_H__
#define __CLASS_IDLE_STATE_H__

//#define IDLESTATE CIdleState::GetSingletonPtr()

//#include "Utils\Singleton.h"

#include "StatesMachine\State.h"
#include "../Characters/Character.h"

//--- Foward Declarations ---//
//class CCharacter;
//---------------------------//


class CIdleState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors ----------------
					CIdleState	( void );
					~CIdleState	( void );

	// ------------- Mètodes Principals ---------------------
	void	Execute		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado es entrado
	void	OnEnter		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado sale
	void	OnExit		( CCharacter* _pCharacter );

	bool	OnMessage	( CCharacter* _pCharacter, const Telegram& );
	
	// ------------- Membres -----------------------------------
private:

};

#endif __CLASS_IDLE_STATE_H__