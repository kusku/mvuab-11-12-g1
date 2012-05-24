#pragma once

#ifndef __CLASS_ANIMATION_WOLF_IDLE_STATE_H__
#define __CLASS_ANIMATION_WOLF_IDLE_STATE_H__

#include "StatesMachine\State.h"


//--- Foward Declarations ---//
class CCharacter;
//---------------------------//


class CAnimationWolfIdleState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors ----------------
					CAnimationWolfIdleState		( void );
					~CAnimationWolfIdleState	( void );

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

#endif __CLASS_ANIMATION_WOLF_IDLE_STATE_H__