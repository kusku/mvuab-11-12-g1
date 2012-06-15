#pragma once

#ifndef __CLASS_STEERING_ENTITY_H__
#define __CLASS_STEERING_ENTITY_H__

#include "Math\Vector2.h"
#include "Math\MathUtils.h"

//--- Foward Declarations ---
class CBaseGameEntity;
//---------------------------

class CSteeringEntity
{
public:
	//--- Init and End protocols------------------------------------------
			CSteeringEntity		(void);
			~CSteeringEntity	(void);

	//---- Main Functions ---------------------------------------
	virtual	Vect2f			CalculateSteering	( CBaseGameEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	
	//---- Register Methods -------------------------------------
	
	
	//---- Properties ( get & Set )---------------------------------------
	inline void					SetVelocity					( const Vect2f &_Velocity )					{ m_Velocity = _Velocity; }
	inline const Vect2f			GetVelocity					( void ) const								{ return m_Velocity; }

	inline void					SetPosition					( const Vect2f &_Position )					{ m_Position = _Position; }
	inline const Vect2f			GetPosition					( void ) const								{ return m_Position; }

	inline void					SetPreviousPosition			( const Vect2f &_PreviousPosition )			{ m_PreviousPosition = _PreviousPosition; }
	inline const Vect2f			GetPreviousPosition			( void ) const								{ return m_PreviousPosition; }

	inline void					SetMaxSpeed					( float _MaxSpeed )							{ m_MaxSpeed = _MaxSpeed; }
	inline const float			GetMaxSpeed					( void ) const								{ return m_MaxSpeed; }

	inline void					SetBoundingRadius			( float _BoundingRadius )					{ m_BoundingRadius = _BoundingRadius; }
	inline const Vect2f			GetBoundingRadius			( void ) const								{ return m_BoundingRadius; }

	// Devuelvo el angulo entre la posición y el objetivo
	inline const float			GetAngle					( void ) const								{ return Vect2f(m_PreviousPosition.y - m_Position.y, m_PreviousPosition.x - m_Position.x).GetAngle(); }

	// Devuelvo el vector en 2D en rotación al eje Y - YAW - donde mira la entidad
	inline const Vect2f			GetFront					( void ) const								{ return Vect2f( mathUtils::Abs(mathUtils::Cos(GetAngle())), mathUtils::Abs(mathUtils::Sin(GetAngle()))); }

	// Devuelvo el vector 2D perpendicular
	inline const Vect2f			GetPerpendicular			( const Vect2f &_Posicion ) const			{ return Vect2f( -_Posicion.y, _Posicion.x ); }

	// Devuelvo el vector 2D perpendicular al Front
	inline const Vect2f			GetSide						( void ) const								{ return GetPerpendicular(GetFront()); }

	//---- Members -----------------------------------------------
private:
		Vect2f	m_Velocity;
        Vect2f	m_Position;
        Vect2f	m_PreviousPosition;
        float	m_MaxSpeed;
        float	m_BoundingRadius;

};

#endif __CLASS_STEERING_ENTITY_H__