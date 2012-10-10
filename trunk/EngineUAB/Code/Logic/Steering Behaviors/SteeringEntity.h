#pragma once

#ifndef __CLASS_STEERING_ENTITY_H__
#define __CLASS_STEERING_ENTITY_H__

#include <string>
#include "Math\Vector3.h"
#include "Math\MathUtils.h"
#include "Smoother.h"

//--- Foward Declarations ---
class CPhysicController;
//---------------------------

class CSteeringEntity
{
public:
	//--- Init and End protocols------------------------------------------
			CSteeringEntity		(void);
			~CSteeringEntity	(void);

	//---- Main Functions ---------------------------------------

	//---- Functions ---------------------------------------
	virtual inline const Vect3f	GetInitialPositionToThrowRay( void ) const;
	virtual inline const Vect3f GetFinalPositionToThrowRay	( float _DegresOfRotation ) const;
		
	//---- Properties ( get & Set )---------------------------------------
	inline const std::string&	GetName						( void ) const							{ return m_Name; }
	inline void					SetName						( const std::string & _Name )			{ m_Name = _Name; }

	inline void					SetVelocity					( const Vect3f &_Velocity )				{ 
																										m_Velocity = _Velocity; 
																									}
	inline const Vect3f&		GetVelocity					( void ) const							{ return m_Velocity; }
	
	// Es la velocidad normalizada
	inline void					SetHeading					( const Vect3f &_Heading )				{ m_Heading = _Heading; }
	inline const Vect3f			GetHeading					( void ) const							{ return m_Heading; }

	inline void					SetPosition					( const Vect3f &_Position )				{ 
																										SetPreviousPosition(m_Position);
																										m_Position = _Position; 
																									}

	inline const Vect3f			GetPosition					( void ) const							{ return m_Position; }

	inline void					SetPreviousPosition			( const Vect3f &_PreviousPosition )		{ 
																										if ( m_PreviousPosition != _PreviousPosition )
																											m_PreviousPosition = _PreviousPosition; 
																									}

	inline const Vect3f			GetPreviousPosition			( void ) const							{ return m_PreviousPosition; }

	inline void					SetMaxSpeed					( float _MaxSpeed )						{ m_MaxSpeed = _MaxSpeed; }
	inline const float			GetMaxSpeed					( void ) const							{ return m_MaxSpeed; }

	inline void					SetBoundingRadius			( float _BoundingRadius )				{ m_BoundingRadius = _BoundingRadius; }
	inline float				GetBoundingRadius			( void ) const							{ return m_BoundingRadius; }

	inline void					SetHeight					( float _Height )						{ m_Height = _Height; }
	inline float				GetHeight					( void ) const							{ return m_Height; }

	inline void					SetMass						( float _Mass )							{ m_Mass = _Mass; }
	inline float				GetMass						( void ) const							{ return m_Mass; }

	inline void					SetMaxForce					( float _MaxForce )						{ m_MaxForce = _MaxForce; }
	inline float				GetMaxForce					( void ) const							{ return m_MaxForce; }

	inline void					SetMaxRotation				( float _MaxTurnRate )					{ m_MaxTurnRate = _MaxTurnRate; }
	inline float				GetMaxRotation				( void ) const							{ return m_MaxTurnRate; }

	inline void					SetMaxAcceleration			( float _MaxAcceleration )				{ m_MaxAcceleration = _MaxAcceleration; }
	inline float				GetMaxAcceleration			( void ) const							{ return m_MaxAcceleration; }

	// Devuelvo la velocidad como float
	inline const float			GetSpeed					( void ) const							{ return  m_Velocity.Length(); }

	// Devuelvo el angulo entre la posición y el objetivo en radianes			
	inline void					SetYaw						( float _Yaw )							{ m_Yaw = _Yaw; }
	inline const float			GetYaw						( void ) const							{ return m_Yaw; }
	
	inline const float			GetAngle					( void ) const							{ if ( m_Position != m_PreviousPosition )
																										return (m_Position - m_PreviousPosition).GetAngleY(); 
																									  else
																										return m_Yaw;	
																									}

	inline const float			GetAngle2					( void ) const							{ return m_Velocity.GetAngleY(); }
	//inline const float			GetAngle3					( void ) const							{ return m_Velocity.GetAngle(); }

	// Devuelvo el vector en 2D en rotación al eje Y - YAW - donde mira la entidad
	///inline const Vect2f			GetFront					( void ) const							{ return Vect2f( mathUtils::Abs(mathUtils::Cos(GetAngle())), mathUtils::Abs(mathUtils::Sin(GetAngle()))); }
																													//Vect3f(mathUtils::Cos<float>(m_fYaw), 0.0, mathUtils::Sin<float>(m_fYaw));
	
	inline const Vect3f			GetFront						( void ) const						{ 
																										Vect3f l_Front; 
																										l_Front.GetXZFromAngle( GetYaw() ); 
																										return l_Front; 
																									}


	// Devuelvo el vector 2D perpendicular
	//inline const Vect2f			GetPerpendicular			( const Vect2f &_Posicion ) const		{ return Vect2f( -_Posicion.y, _Posicion.x ); }
	
	inline void					SetSide						( const Vect3f &_Side )						{ m_Side = _Side; }
	inline const Vect3f			GetSide						( void ) const								{ return m_Side; }

	inline CPhysicController*	GetController				( void )									{ return m_pController; }
	inline void					SetController				( CPhysicController* _pController )			{ m_pController = _pController; }
			
	// --- Smooth movement ---
	inline void					SetSmoothedHeading			( const Vect3f &_SmoothingVector ) 			{ m_SmoothedHeading = _SmoothingVector; }
	inline const Vect3f			GetSmoothedHeading			( void ) const								{ return m_SmoothedHeading; }

	inline CSmoother<Vect3f>*	GetHeadingSmoother			( void ) 									{ return m_pHeadingSmoother; }

	inline bool					isSmoothingOn				( void ) const								{ return m_SmoothingOn; }
	inline void					SmoothingOn					( void )									{ m_SmoothingOn = true; }
	inline void					SmoothingOff				( void )									{ m_SmoothingOn = false; }
	inline void					ToggleSmoothing				( void )									{ m_SmoothingOn = !m_SmoothingOn; }
	// ----------------------- 

	//---- Members -----------------------------------------------
private:
		std::string			m_Name;					// Nom entitat
		float				m_Mass;					// Massa de la entidad
		float				m_Yaw;					// Angulo de rotación 
		Vect3f				m_Velocity;				// Vector de velocidad
        Vect3f				m_Position;				// Posición de la entidad
        Vect3f				m_PreviousPosition;		// Posición anterior de la entidad
		float				m_MaxAcceleration;		// Máxima aceleracion permitida de la entidad
        float				m_MaxSpeed;				// Máxima velocidad permitida de la entidad
		float				m_MaxForce;				// Máxima fuerza permitida de la entidad
		float				m_MaxTurnRate;			// Velocidad Máxima de giro permitida de la entidad en radians per segon
        float				m_BoundingRadius;		// Radio de encapçulamiento de todo el objeto
		float				m_Height;				// Altura de la entidad
		Vect3f				m_Heading;				// Vector direcció de la velocidad = GetFront
		Vect3f				m_Side;					// Vector perpendicular al heading 
		
		// --- Smooth members ---
		Vect3f				m_SmoothedHeading;		// Vector que representa la media de headings de los ultimos frames i que nos permitirá suavizar los movimientos evitando el temblor
		bool				m_SmoothingOn;			// Dice si lo activamos
		CSmoother<Vect3f>*	m_pHeadingSmoother;		// Listado de los headings de los ultimos frames
		// ----------------------

		CPhysicController*	m_pController;			// Controller de physX. A veces necesito acceder a él
};

#endif __CLASS_STEERING_ENTITY_H__