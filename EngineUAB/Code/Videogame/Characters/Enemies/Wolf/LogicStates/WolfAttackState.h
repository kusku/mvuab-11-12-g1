#pragma once

#ifndef _WOLF_ATTACK_STATE_H_
#define _WOLF_ATTACK_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
// ---------------------------

class CWolfAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfAttackState	( CCharacter* );
					CWolfAttackState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter env�a un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	std::string		GetRandomAttackName	(void);

	//----Members ---------------------------------------------
private:
	CActionStateCallback		m_ActionTime;
	CWolf					  * m_pWolf;
	
	bool						m_bInPositionToAttack;		// Me dice que si he llegado a las posiciones de ataque
	Vect3f						m_SearchedAttackPoint;		// Punto al cual el caracter se va a situar
	float						m_AngleRangeFromCamara;		// Angulo en el qual el enemigo puede situarse dentro del frustum. Se buscar� una posici�n dentro de este rango dentro el front de c�mara
};


#endif _WOLF_ATTACK_STATE_H_