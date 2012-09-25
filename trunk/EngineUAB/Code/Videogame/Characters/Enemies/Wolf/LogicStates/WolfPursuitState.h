#pragma once

#ifndef _WOLF_PURSUIT_STATE_H_
#define _WOLF_PURSUIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CWolfPursuitState	( CCharacter* );
					CWolfPursuitState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfPursuitState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			UpdateParticles		( CCharacter* _pCharacter );
	void			StopParticles		( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CWolf				  * m_pWolf;
	CActionStateCallback	m_SoundActionStateCallback;
	CActionStateCallback	m_RunActionStateCallback;
	float					m_SoundDuration;

	bool					m_FirtsStepDone, m_SecondStepDone;
};


#endif _WOLF_PURSUIT_STATE_H_