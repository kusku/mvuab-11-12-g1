#pragma once

#ifndef __CLASS_PURSUIT_H__
#define __CLASS_PURSUIT_H__

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
class CArrive;
class CSeek;
//---------------------------

class CPursuit : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							CPursuit			( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance );
							CPursuit			( void );
	virtual					~CPursuit			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    void					UpdateEvaderEntity	( CSteeringEntity *_pEvader );

	//---- Functions ---------------------------------------
	
	//---- Properties ( get & Set )---------------------------------------

	//---- Members -----------------------------------------------
private:
		bool				m_EvaderUpdated;
		CArrive			*	m_pArrive;
        CSteeringEntity *	m_pEvader;
		CSeek			*	m_pSeek;
		eDeceleration		m_Deceleration;
		float				m_DecelerationDistance;
};

#endif __CLASS_PURSUIT_H__