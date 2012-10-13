#pragma once

#ifndef _RABBIT_PREPARED_TO_ATTACK_STATE_H_
#define _RABBIT_PREPARED_TO_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitPreparedToAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitPreparedToAttackState	( CCharacter* );
					CRabbitPreparedToAttackState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitPreparedToAttackState	( void );

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
	
	//----Members ---------------------------------------------
private:
	CRabbit	 *	m_pRabbit;

	bool		m_IsPositionAssignedAfterHitPlayer;
	Vect3f		m_PositionReachedAfterHitPlayer;
	float		m_AngleRangeFromCamara;		// Angulo en el qual el enemigo puede situarse dentro del frustum. Se buscará una posición dentro de este rango dentro el front de cámara
};


#endif _RABBIT_PREPARED_TO_ATTACK_STATE_H_