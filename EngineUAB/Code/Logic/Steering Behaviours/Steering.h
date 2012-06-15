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
							CSteering			( BehaviorType _Type );
	virtual					~CSteering			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect2f			CalculateSteering	( CSteeringEntity *_pEntity )	= 0;				 // Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	inline BehaviorType		GetType				( void ) const		{ return m_Type; }		// Tipo del steering concreto, se establece en el constructor

	//---- Register Methods -------------------------------------
private:	
	static void				RegisterLUAMethods	( void );

protected:

	BehaviorType m_Type;		// Tipo del steering concreto
};

#endif __STEERING_CLASS_H__