#pragma once

#ifndef __STEERING_CLASS_H__
#define __STEERING_CLASS_H__

#include "Math\Vector2.h"
#include "SteeringBehavioursDefs.h"

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
	virtual	Vect2f			CalculateSteering	( CSteeringEntity *_pEntity )	= 0;				 // Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	inline eBehaviorType	GetType				( void ) const							{ return m_Type; }		// Tipo del steering concreto, se establece en el constructor
	
	inline void				SetTarget				( const Vect2f&	_Target )			{ m_Target = _Target; }
	inline const Vect2f&	GetTarget				( void ) const						{ return m_Target; }		
	
	//---- Register Methods -------------------------------------
	static void				RegisterLUAMethods	( void );

protected:

	eBehaviorType	m_Type;		// Tipo del steering concreto
	Vect2f			m_Target;
};

#endif __STEERING_CLASS_H__