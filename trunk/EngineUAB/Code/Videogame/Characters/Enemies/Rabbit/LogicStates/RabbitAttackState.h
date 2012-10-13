#pragma once

#ifndef _RABBIT_ATTACK_STATE_H_
#define _RABBIT_ATTACK_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitAttackState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitAttackState	( CCharacter* );
					CRabbitAttackState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitAttackState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	std::string		GetRandomAttackName	(void);

	//----Members ---------------------------------------------
private:
	CActionStateCallback		m_ActionTime;
	CRabbit					  * m_pRabbit;
	
	bool						m_bInPositionToAttack;		// Me dice que si he llegado a las posiciones de ataque
	Vect3f						m_SearchedAttackPoint;		// Punto al cual el caracter se va a situar
	float						m_AngleRangeFromCamara;		// Angulo en el qual el enemigo puede situarse dentro del frustum. Se buscará una posición dentro de este rango dentro el front de cámara
};


#endif _RABBIT_ATTACK_STATE_H_