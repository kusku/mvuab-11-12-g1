#pragma once

#ifndef __CLASS_IDLE_STATE_H__
#define __CLASS_IDLE_STATE_H__

#define IDLESTATE CIdleState::GetSingletonPtr()

#include "Utils\Singleton.h"

#include "State.h"
//#include "BaseGameEntity.h"

//--- Foward Declarations ---//
class CCharacter;
//---------------------------//


class CIdleState : public CSingleton<CIdleState>, public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors ----------------
					CIdleState	( void );
					~CIdleState	( void );

	// ------------- Mètodes Principals ---------------------
	virtual void	Execute		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* _pCharacter );

	// ------------- Membres -----------------------------------
private:

};

#endif __CLASS_IDLE_STATE_H__