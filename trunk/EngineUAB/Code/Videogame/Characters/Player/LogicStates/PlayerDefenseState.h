#pragma once

#ifndef _PLAYER_DEFENSE_STATE_H_
#define _PLAYER_DEFENSE_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;
class CActionToInput;

class CPlayerDefenseState : public CState< CCharacter >
{
public:
	CPlayerDefenseState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerDefenseState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

	void	CalculateRecoilDirection	( CCharacter * _pCharacter );

	void	SetDoubleHit				( bool _DoubleHit )			{ m_DoubleHit = _DoubleHit; }
	bool	GetDoubleHit				( void ) const				{ return m_DoubleHit; }

private:
	CActionToInput *m_pInput;

	Vect3f					m_HitDirection;			// Dirección hacia atrás donde se dirige en el golpeo
	Vect3f					m_HitMaxPosition;		// Posición final del golpeo
	float					m_MaxHitDistance;		// Distancia máxima de recorrido del hit
	float					m_MaxHitSpeed;			// Velocidad máxima de recorrido del hit
	Vect3f					m_InitialHitPoint;		// Punto final de retroceso

	STelegram				m_Message;				// Mensaje del impacto
	CCharacter *			m_pEnemy;				// Enemigo que me pega

	bool					m_DoubleHit;			// Indica si se ha recibido otro golpe después del primero
	bool					m_IsAttacked;			// Indica si se ha recibido ya el Push o Attack
	bool					m_PushHit;				// Indica si se ha recibido el Push 
};

#endif //_PLAYER_DEFENSE_STATE_H_