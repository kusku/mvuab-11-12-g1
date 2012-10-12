#pragma once

#ifndef _DEER_HIT_STATE_H_
#define _DEER_HIT_STATE_H_

#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CDeer;
class CAnimationCallback;
// ---------------------------

class CDeerHitState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CDeerHitState			( CCharacter* );
					CDeerHitState			( CCharacter*, const std::string &_Name );
	virtual			~CDeerHitState			( void );

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

	void			CalculateRecoilDirection( CCharacter * _pCharacter );
	
	void			UpdateParameters		( STelegram& _Message );

	//----Properties ( get & Set )-----------------------------
	inline void					SetDoubleHit	( bool _DoubleHit )				{ m_DoubleHit = _DoubleHit; }
	inline const bool			GetDoubleHit	( void ) const					{ return m_DoubleHit; }
	
	//----Members ---------------------------------------------
private:
	CDeer					  * m_pDeer;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback		m_pActionStateCallback;
	
	float						m_ActionDuration;
	bool						m_IsCommingFromTired;
	float						m_RecoverMinTiredTime;
	float						m_RecoverMaxTiredTime;
	
	Vect3f						m_HitDirection;			// Dirección hacia atrás donde se dirige en el golpeo
	Vect3f						m_HitMaxPosition;		// Posición final del golpeo
	float						m_MaxHitDistance;		// Distancia máxima de recorrido del hit
	float						m_MaxHitSpeed;			// Velocidad máxima de recorrido del hit
	Vect3f						m_InitialHitPoint;		// Punto final de retroceso

	STelegram					m_Message;				// Mensaje del impacto
	CCharacter *				m_pEnemy;				// Enemigo que me pega

	bool						m_DoubleHit;			// Permite percibir el doble golpeo
};
#endif _DEER_HIT_STATE_H_