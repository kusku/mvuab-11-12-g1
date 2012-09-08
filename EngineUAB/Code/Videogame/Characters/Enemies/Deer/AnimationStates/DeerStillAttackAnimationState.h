#pragma once

#ifndef _DEER_STILL_ATTACK_ANIMATION_STATE_H_
#define _DEER_STILL_ATTACK_ANIMATION_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CDeer;
// ---------------------------

class CDeerStillAttackAnimationState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CDeerStillAttackAnimationState	( void );
					CDeerStillAttackAnimationState	( const std::string &_Name );
	virtual			~CDeerStillAttackAnimationState	( void );

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
	inline CDeer * GetCharacter( void )		{ return m_pDeer; }

	//----Members ---------------------------------------------
private:
	CDeer *		m_pDeer;

};


#endif _DEER_STILL_ATTACK_ANIMATION_STATE_H_