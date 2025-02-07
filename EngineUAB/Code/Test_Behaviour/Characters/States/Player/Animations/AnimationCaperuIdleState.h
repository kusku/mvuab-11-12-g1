#pragma once

#ifndef __CLASS_ANIMATION_CAPERU_IDLE_STATE_H__
#define __CLASS_ANIMATION_CAPERU_IDLE_STATE_H__

#include "StatesMachine\State.h"


//--- Foward Declarations ---//
class CCharacter;
//---------------------------//


class CAnimationCaperuIdleState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors ----------------
					CAnimationCaperuIdleState		( void );
					~CAnimationCaperuIdleState	( void );

	// ------------- M�todes Principals ---------------------
	void	Execute		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado es entrado
	void	OnEnter		( CCharacter* _pCharacter );

	// Se ejecuta cuando el estado sale
	void	OnExit		( CCharacter* _pCharacter );

	bool	OnMessage	( CCharacter* _pCharacter, const Telegram& );
	
	// ------------- Membres -----------------------------------
private:

};

#endif __CLASS_ANIMATION_CAPERU_IDLE_STATE_H__