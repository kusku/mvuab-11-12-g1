#pragma once

#ifndef _DEER_DEATH_STATE_H_
#define _DEER_DEATH_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
// ---------------------------

class CDeerDeathState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerDeathState			( CCharacter* _pCharacter );
					CDeerDeathState			( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CDeerDeathState		( void );

	//----Main Functions --------------------------------------
	virtual void	Execute					( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter					( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit					( CCharacter* );

	// Se ejecuta cuando otro caracter env�a un mensaje a este
	virtual bool	OnMessage				( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	// Para tema de part�culas de impacto
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );
	void			StopImpact				( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CDeer					  * m_pDeer;
	CAnimationCallback		  *	m_pAnimationCallback;

	float						m_MaxHitDistance;		// Me dice la distancia que recorro cuando pega y bloqueo hacia atras
	Vect3f						m_InitialHitPoint;		// Punto inicial de retroceso
	Vect3f						m_HitDirection;			// Direcci�n hacia atr�s donde se dirige en el golpeo
};
#endif _DEER_DEATH_STATE_H_