#pragma once

#ifndef _RABBIT_DEFENSE_STATE_H_
#define _RABBIT_DEFENSE_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitDefenseState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitDefenseState	( CCharacter* );
					CRabbitDefenseState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitDefenseState( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	// Para tema de partículas de impacto
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );
	void			StopImpact				( CCharacter* _pCharacter );


	//----Properties ------------------------------------------
	const bool		IsHitBlocked	( void )		{ return m_HitIsBlocked; }

	//----Members ---------------------------------------------
private:
	CActionStateCallback		m_ActionTime;
	CRabbit					  * m_pRabbit;

	bool						m_HitIsBlocked;			// Me dice si bloqueo
	float						m_MaxHitDistance;		// Me dice la distancia que recorro cuando pega y bloqueo hacia atras
	Vect3f						m_InitialHitPoint;		// Punto inicial de retroceso
	Vect3f						m_HitDirection;			// Dirección hacia atrás donde se dirige en el golpeo

	int							m_TotalHitBlocked;		// Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
	int							m_HitBlockedCount;	// Me dice el nº de veces que el player me pega mientras bloqueo
};
#endif _RABBIT_DEFENSE_STATE_H_