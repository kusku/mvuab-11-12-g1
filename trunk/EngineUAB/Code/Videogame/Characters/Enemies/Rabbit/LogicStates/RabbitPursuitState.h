#pragma once

#ifndef _RABBIT_PURSUIT_STATE_H_
#define _RABBIT_PURSUIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitPursuitState : public CState<CCharacter>
{
public:
	// ------------- Constructors i Destructors --------------
					CRabbitPursuitState		( CCharacter* );
					CRabbitPursuitState		( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitPursuitState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			PlaySequencialSound	( int _NumSoundToPlay );

	//----Members ---------------------------------------------
private:
	CRabbit				*	m_pRabbit;
	CActionStateCallback	m_ActionStateCallback;
	float					m_SoundDuration;
	int						m_CurrentSoundToPlay;		// Esta rotación de sonidos la haré secuencial para encarjarlos
};


#endif _RABBIT_PURSUIT_STATE_H_