#pragma once

#ifndef _DEER_STILL_ATTACK_STATE_H_
#define _DEER_STILL_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
// ---------------------------

class CDeerStillAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerStillAttackState	( void );
					CDeerStillAttackState	( const std::string &_Name );
	virtual			~CDeerStillAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------

	//----Members ---------------------------------------------
private:
	CDeer					  * m_pDeer;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback		m_pActionStateCallback;

	//bool						m_PlayerReached;		// Indica si el golpeo alcanza el player
	bool						m_SoundPlayed1;			// Indica si el sonido ya se inició
	bool						m_SoundPlayed2;			// Indica si el sonido ya se inició
};


#endif _DEER_STILL_ATTACK_STATE_H_