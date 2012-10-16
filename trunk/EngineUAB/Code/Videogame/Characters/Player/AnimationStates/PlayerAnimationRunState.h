#pragma once

#ifndef _PLAYER_ANIMATION_RUN_STATE_H_
#define _PLAYER_ANIMATION_RUN_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

class CActionStateCallback;

struct STelegram;

class CPlayerAnimationRunState : public CState< CCharacter >
{
public:
	// ------------- Constructors i Destructors --------------
	CPlayerAnimationRunState( CCharacter* _pCharacter, const std::string &_Name );
	~CPlayerAnimationRunState();

	//----Main Functions --------------------------------------
	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

	//----Members ---------------------------------------------
private:	
	CActionStateCallback		* m_ActionCallback;

	float	m_AnimationDuration;

};

#endif //_PLAYER_ANIMATION_RUN_STATE_H_