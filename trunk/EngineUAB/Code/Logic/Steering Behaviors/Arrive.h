#pragma once

#ifndef __CLASS_ARRIVE_H__
#define __CLASS_ARRIVE_H__

#include "Math\Vector2.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

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
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// M�todo que deber� ser sobreescrito en cada clase que herede de Steering

	//---- Members -----------------------------------------------
private:
		eDeceleration	m_Deceleration;
		float	 		m_DecelerationBeginsAtDistance;
};

#endif __CLASS_ARRIVE_H__