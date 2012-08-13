#pragma once

#ifndef __STEERING_CLASS_H__
#define __STEERING_CLASS_H__

#include "Math\Vector3.h"
#include "SteeringBehaviorsDefs.h"

// --- Foward Declaractions ---
//class CObject3D;
class CSteeringEntity;
// ----------------------------

class CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							CSteering			( eBehaviorType _Type );
	virtual					~CSteering			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity )	= 0;						// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	inline eBehaviorType	GetType				( void ) const						{ return m_Type; }		// Tipo del steering concreto, se establece en el constructor
	
	inline void				SetTarget			( const Vect3f&	_Target )			{ 
																						m_Target = _Target; 
																					}
	inline const Vect3f&	GetTarget			( void ) const						{ 
																						return m_Target; 
																					}		
	
	inline void				SetTargetVelocity	( const Vect3f&	_Velocity )			{ 
																						m_TargetVelocity = _Velocity; 
																					}
	inline const Vect3f&	GetTargetVelocity	( void ) const						{ 
																						return m_TargetVelocity; 
																					}		

protected:

	eBehaviorType			m_Type;				// Tipo del steering concreto
	Vect3f					m_Target;			// Posicion del objetivo
	Vect3f					m_TargetVelocity;	// Velocidad del objetivo
};

#endif __STEERING_CLASS_H__