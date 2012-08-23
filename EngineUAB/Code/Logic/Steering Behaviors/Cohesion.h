#pragma once

#ifndef __CLASS_COHESION_H__
#define __CLASS_COHESION_H__

#include <vector>

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
class CPhysicUserData;
class CSeek;
//---------------------------

class CCohesion : public CSteering
{
public:


	//--- Init and End protocols------------------------------------------
							CCohesion			( void );
	virtual					~CCohesion			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------
	void					UpdateParameters	( const std::vector<CPhysicUserData*> &_UserDatas );

	//---- Properties ( get & Set )---------------------------------------
	
	//---- Members -----------------------------------------------
private:
	std::vector<CPhysicUserData*>	m_UserDatas;
	bool							m_CohesionUpdated;
	CSeek						*	m_pSeek;
};

#endif __CLASS_COHESION_H__