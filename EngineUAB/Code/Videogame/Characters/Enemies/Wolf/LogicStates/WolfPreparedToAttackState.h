#pragma once

#ifndef _WOLF_PREPARED_TO_ATTACK_STATE_H_
#define _WOLF_PREPARED_TO_ATTACK_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfPreparedToAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfPreparedToAttackState	( CCharacter* );
					CWolfPreparedToAttackState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfPreparedToAttackState	( void );

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
	CWolf	  * m_pWolf;

	bool		m_IsPositionAfterHitPlayerAssigned;
	Vect3f		m_PositionReachedAfterHitPlayer;
	float		m_AngleRangeFromCamara;		// Angulo en el qual el enemigo puede situarse dentro del frustum. Se buscará una posición dentro de este rango dentro el front de cámara
};


#endif _WOLF_PREPARED_TO_ATTACK_STATE_H_