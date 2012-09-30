#pragma once

#ifndef _RABBIT_DEATH_ATTACK_STATE_H_
#define _RABBIT_DEATH_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
class CAnimationCallback;
// ---------------------------

class CRabbitDeathState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitDeathState	( CCharacter* );
					CRabbitDeathState	( CCharacter*, const std::string &_Name );
	virtual			~CRabbitDeathState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	//void			PlayRandomSound	( void );
	
	//----Members ---------------------------------------------
private:
	CRabbit					  * m_pRabbit;
	CAnimationCallback		  *	m_pAnimationCallback;
	float						m_SoundDuration;
};
#endif _RABBIT_DEATH_ATTACK_STATE_H_