#pragma once

#ifndef __CLASS_ARRIVE_H__
#define __CLASS_ARRIVE_H__

#include "Math\Vector2.h"
#include "Steering Behaviours\Steering.h"
#include "SteeringBehavioursDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
//---------------------------

class CArrive : public CSteering
{
public:


	//--- Init and End protocols------------------------------------------
							CArrive				( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance );
	virtual					~CArrive			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect2f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	
	//---- Register Methods -------------------------------------
	static void				RegisterLUAMethods	( void );

	//---- Members -----------------------------------------------
private:
		eDeceleration	m_Deceleration;
		float	 		m_DecelerationBeginsAtDistance;
};

#endif __CLASS_ARRIVE_H__