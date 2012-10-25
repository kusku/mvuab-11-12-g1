#pragma once

#ifndef _PLAYER_DEATH_STATE_H_
#define _PLAYER_DEATH_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CPlayer;
class CAnimationCallback;
// ---------------------------

class CPlayerDeathState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CPlayerDeathState			( CCharacter* _pCharacter );
					CPlayerDeathState			( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CPlayerDeathState		( void );

	//----Main Functions --------------------------------------
	virtual void	Execute					( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter					( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit					( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage				( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	// Para tema de partículas de impacto
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );
	void			StopImpact				( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CPlayer					  * m_pPlayer;
	CAnimationCallback		  *	m_pAnimationCallback;

	float						m_MaxHitDistance;		// Me dice la distancia que recorro cuando pega y bloqueo hacia atras
	Vect3f						m_InitialHitPoint;		// Punto inicial de retroceso
	Vect3f						m_HitDirection;			// Dirección hacia atrás donde se dirige en el golpeo
};
#endif _PLAYER_DEATH_STATE_H_