#pragma once

#ifndef _WOLF_ONLY_RUN_ATTACK_ANIMATION_STATE_H_
#define _WOLF_ONLY_RUN_ATTACK_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfRunRunAttackAnimationState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CWolfRunRunAttackAnimationState	( CCharacter* );
					CWolfRunRunAttackAnimationState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CWolfRunRunAttackAnimationState	( void );

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
	inline CWolf * GetCharacter( void )		{ return m_pWolf; }

	//----Members ---------------------------------------------
private:
	CWolf *		m_pWolf;

	float			m_AnimationDuration;
	float			m_Currentduration;

};


#endif _WOLF_ONLY_RUN_ATTACK_ANIMATION_STATE_H_