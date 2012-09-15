#pragma once

#ifndef _DEER_PURSUIT_STATE_H_
#define _DEER_PURSUIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
// ---------------------------

class CDeerPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CDeerPursuitState		( void );
					CDeerPursuitState		( const std::string &_Name );
	virtual			~CDeerPursuitState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute			( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter			( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit			( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage		( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			PlayRandomSound	( void );

	//----Members ---------------------------------------------
private:
	CDeer				  * m_pDeer;
	CActionStateCallback	m_ActionCallback;
	float					m_SoundDuration;
};


#endif _DEER_PURSUIT_STATE_H_