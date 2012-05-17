#pragma once

#ifndef __CLASS_PURSUIT_STATE_H__
#define __CLASS_PURSUIT_STATE_H__

//#define PURSUITSTATE CPursuitState::GetSingletonPtr()

//#include "Utils\Singleton.h"

#include "StatesMachine\State.h"
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
	void	Execute		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado es entrado
	void	OnEnter		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado sale
	void	OnExit		( CCharacter* _pCharacter );

	bool	OnMessage	( CCharacter* _pCharacter, const Telegram& );

	// ------------- Membres -----------------------------------
};

#endif __CLASS_PURSUIT_STATE_H__