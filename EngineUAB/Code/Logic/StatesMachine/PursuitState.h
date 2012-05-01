#pragma once

#ifndef __CLASS_PURSUIT_STATE_H__
#define __CLASS_PURSUIT_STATE_H__

//#define PURSUIT CPursuitState::GetSingletonPtr()

#include "Utils\Singleton.h"

#include "State.h"
//#include "BaseGameEntity.h"

//--- Foward Declarations ---//
class CCharacter;
//---------------------------//

class CPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors ----------------
					CPursuitState	( void );
					~CPursuitState	( void );

	// ------------- Mètodes Principals ---------------------
	virtual void	Execute		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* _pCharacter );

	// ------------- Membres -----------------------------------
};

#endif __CLASS_PURSUIT_STATE_H__