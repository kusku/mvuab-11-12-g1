#pragma once

#ifndef __CLASS_ANIMATION_PURSUIT_STATE_H__
#define __CLASS_ANIMATION_PURSUIT_STATE_H__

//#define PURSUITSTATE AnimationPursuitState::GetSingletonPtr()

//#include "Utils\Singleton.h"

#include "StatesMachine\State.h"
//#include "BaseGameEntity.h"

//--- Foward Declarations ---//
class CCharacter;
//---------------------------//

class CAnimationPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors ----------------
					CAnimationPursuitState	( void );
					~CAnimationPursuitState	( void );

	// ------------- Mètodes Principals ---------------------
	void	Execute		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado es entrado
	void	OnEnter		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado sale
	void	OnExit		( CCharacter* _pCharacter );

	bool	OnMessage	( CCharacter* _pCharacter, const Telegram& );

	// ------------- Membres -----------------------------------
};

#endif __CLASS_ANIMATION_PURSUIT_STATE_H__