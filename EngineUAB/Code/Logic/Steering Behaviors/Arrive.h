#pragma once

#ifndef __CLASS_ARRIVE_H__
#define __CLASS_ARRIVE_H__

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
//---------------------------

class CArrive : public CSteering
{
public:


	//--- Init and End protocols------------------------------------------
								CArrive					( void );
								CArrive					( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance );
	virtual						~CArrive				( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f				CalculateSteering		( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	inline void					SetDeceleration			( const eDeceleration _Deceleration )			{ m_Deceleration = _Deceleration; }
	inline const eDeceleration	GetDeceleration			( void ) const									{ return m_Deceleration; }
	
	inline void					SetDecelerationDistance	( float _DecelerationBeginsAtDistance )			{ m_DecelerationBeginsAtDistance = _DecelerationBeginsAtDistance; }
	inline const float			GetDecelerationDistance	( void ) const									{ return m_DecelerationBeginsAtDistance; }
	
	inline void					SetDecelerationTweaker	( float _DecelerationTweaker )					{ m_DecelerationTweaker = _DecelerationTweaker; }
	inline const float			GetDecelerationTweaker	( void ) const									{ return m_DecelerationTweaker; }
	//---- Members -----------------------------------------------
private:
	eDeceleration				m_Deceleration;							// Velocidad a la que desaceleramos
	float	 					m_DecelerationBeginsAtDistance;			// Distancia a partir de la cual desaceleramos
	float	 					m_DecelerationTweaker;					// Un valor para suavizar la desaceleración
};	

#endif __CLASS_ARRIVE_H__