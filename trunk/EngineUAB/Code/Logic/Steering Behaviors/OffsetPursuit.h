#pragma once

#ifndef __CLASS_OFFSET_PURSUIT_H__
#define __CLASS_OFFSET_PURSUIT_H__

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
class CArrive;
class CSeek;
//---------------------------

class COffsetPursuit : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							COffsetPursuit			( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance );
							COffsetPursuit			( void );
	virtual					~COffsetPursuit			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------
	void					UpdateLeaderEntity	( CSteeringEntity *_pLeader, eDeceleration _Deceleration, float _Offset );

	inline Vect3f			PointToWorldSpace	( const Vect3f &_Point, const Vect3f &_AgentHeading, const Vect3f &_AgentSide, const Vect3f &_AgentPosition );

	//---- Properties ( get & Set )---------------------------------------

	//---- Members -----------------------------------------------
private:
		CArrive			*	m_pArrive;
		CSteeringEntity *	m_pLeader;
        bool				m_LeaderUpdated;
		eDeceleration		m_Deceleration;
		float				m_DecelerationDistance;
		float				m_Offset;
};

#endif __CLASS_OFFSET_PURSUIT_H__