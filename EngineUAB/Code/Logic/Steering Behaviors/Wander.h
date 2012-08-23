#pragma once

#ifndef __CLASS_WANDER_H__
#define __CLASS_WANDER_H__

//#include <numeric>
#include "Math\Vector3.h"
#include "Math\MathUtils.h"
//#include "Utils\Random.h"
#include "Steering Behaviors\Steering.h"


//--- Foward Declarations ---
class CSteeringEntity;
class CArrive;
//---------------------------

class CWander : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
								CWander						( void );
								CWander						( float _RefreshRate, float _DistanciaCirculo, float _RadioCirculo, eDeceleration _Deceleration, float _DecelerationBeginsAtDistance );
	virtual						~CWander					( void );

	//---- Main Methods --------------------------------------------------
	virtual	Vect3f				CalculateSteering			( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering

	//---- Methods -------------------------------------------------------
	//returns a random double between zero and 1
	/*inline double				RandFloat					( void )								{	CRandom r;
																										return ((rand())/(r.getRandFloat()+1.0)); 
																									}*/
	
	/*inline float				RandFloat					( void )								{ return ((rand())/(((float)RAND_MAX+1.0)); }


	inline float				RandomClamped				( void )								{ return RandFloat() - RandFloat(); }*/

	//---- Properties ( get & Set )---------------------------------------
	inline void					SetWanderRadius				( float _WanderRadius )					{ m_WanderRadius = _WanderRadius; }
	inline const float			GetWanderRadius				( void ) const							{ return m_WanderRadius; }
	
	inline void					SetWanderDistance			( float _WanderDistance )				{ m_WanderDistance = _WanderDistance; }
	inline const float			GetWanderDistance			( void ) const							{ return m_WanderDistance; }
	
	inline void					SetWanderJitter				( float _WanderJitter )					{ m_WanderJitter = _WanderJitter; }
	inline const float			GetWanderJitter				( void ) const							{ return m_WanderJitter; }
	
	inline void					SetWanderMinimumDistance	( float _WanderMinimumDistance )		{ m_WanderMinimumDistance = _WanderMinimumDistance; }
	inline const float			SetWanderMinimumDistance	( void ) const							{ return m_WanderMinimumDistance; }

	//---- Members -------------------------------------------------------
private:
	float			m_WanderRadius;				// Radio de la esfera 
	float			m_WanderDistance;			// Distancia de la entidad hasta donde es proyectada la esfera
	float			m_WanderMinimumDistance;	// Distancia mínima para volver a cambiar de sentido
	float			m_WanderJitter;				// Máxima cantidad de nervio, de random displacement, que podemos añadir al objetivo
	float			m_Time;						// Variable de uso para el cálculo del refresco
	Vect3f			m_WanderTarget;				// Vector entre 1 i -1 para añadir un random vector a la posición objectivo
	float			m_WanderOrientation;		// Orientación del nuevo target

	CArrive *		m_pArrive;				// Behaviur para desacelerar
};

#endif __CLASS_WANDER_H__