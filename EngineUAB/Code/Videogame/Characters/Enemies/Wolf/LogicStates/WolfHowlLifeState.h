#pragma once

#ifndef _WOLF_HOWL_STATE_H_
#define _WOLF_HOWL_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CWolf;
class CAnimationCallback;
// ---------------------------

class CWolfHowlLifeState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfHowlLifeState	( CCharacter* );
					CWolfHowlLifeState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfHowlLifeState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute			( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter			( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit			( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage		( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			IncreaseLife	( void );

	//----Members ---------------------------------------------
private:
	CWolf				  * m_pWolf;
	CActionStateCallback	m_ActionStateCallback;
	CAnimationCallback    *	m_pAnimationCallback;

	float					m_SoundDuration;
};


#endif _WOLF_HOWL_STATE_H_