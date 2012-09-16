#pragma once

#ifndef _DEER_HIT_STATE_H_
#define _DEER_HIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
// ---------------------------

class CDeerHitState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerHitState	( void );
					CDeerHitState	( const std::string &_Name );
	virtual			~CDeerHitState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			PlayRandomSound( void );

	//----Members ---------------------------------------------
private:
	CDeer					  * m_pDeer;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback		m_pActionState;
	float						m_SoundDuration;

	bool						m_IsCommingFromTired;
	float						m_RecoverMinTiredTime;
	float						m_RecoverMaxTiredTime;
	
};
#endif _DEER_HIT_STATE_H_