#pragma once

#ifndef _RABBIT_IDLE_STATE_H_
#define _RABBIT_IDLE_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitIdleState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitIdleState	( CCharacter* );
					CRabbitIdleState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitIdleState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			UpdateAnimations ( CCharacter*, float _ElapsedTime );
	void			UpdateSounds	 ( CCharacter*, float _ElapsedTime );
	
	//----Members ---------------------------------------------
private:
	CRabbit				  * m_pRabbit;
	CActionStateCallback	m_ActionStateCallback;
	CActionStateCallback	m_SoundActionStateCallback;

	float					m_SoundDuration;
	bool					m_AlreadyDetected;
	bool					m_AlreadyChased;
	bool					m_IdleWarningSounds;
};


#endif _RABBIT_IDLE_STATE_H_