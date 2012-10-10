#pragma once

#ifndef _PLAYER_HIT_STATE_H_
#define _PLAYER_HIT_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;

// --- Foward declarations --- //
class CAnimationCallback;
class CCharacter;
// --------------------------- //

class CPlayerHitState : public CState< CCharacter >
{
public:
	CPlayerHitState						( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerHitState					( void );

	void	OnEnter						(  CCharacter* _pCharacter );
	void	Execute						(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit						(  CCharacter* _pCharacter );

	bool	OnMessage					(  CCharacter* _pCharacter,  const STelegram& _Message );

	void	UpdateParameters			( STelegram& _Message );
	void	CalculateRecoilDirection	( CCharacter * _pCharacter );

	void	SetDoubleHit				( bool _DoubleHit )			{ m_DoubleHit = _DoubleHit; }
	bool	GetDoubleHit				( void ) const				{ return m_DoubleHit; }

private:
	CAnimationCallback	  * m_pCallback;

	Vect3f					m_HitDirection;			// Dirección hacia atrás donde se dirige en el golpeo
	Vect3f					m_HitMaxPosition;		// Posición final del golpeo
	float					m_MaxHitDistance;		// Distancia máxima de recorrido del hit
	float					m_MaxHitSpeed;			// Velocidad máxima de recorrido del hit
	Vect3f					m_InitialHitPoint;		// Punto final de retroceso

	STelegram				m_Message;				// Mensaje del impacto
	CCharacter *			m_pEnemy;				// Enemigo que me pega

	bool					m_DoubleHit;
};

#endif //_PLAYER_HIT_STATE_H_