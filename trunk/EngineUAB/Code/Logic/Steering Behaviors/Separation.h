#pragma once

#ifndef __CLASS_SEPARATION_H__
#define __CLASS_SEPARATION_H__

#include <vector>

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"

#include "PhysicsDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
class CPhysicUserData;

//---------------------------

class CSeparation : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							CSeparation			( void );
	virtual					~CSeparation		( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------
	void					UpdateParameters	( const std::vector<CPhysicUserData*> &_UserDatas );

	//---- Properties ( get & Set )---------------------------------------
	//---- Members -----------------------------------------------
private:
	std::vector<CPhysicUserData*>	m_UserDatas;
	bool							m_SeparationUpdated;

};

#endif __CLASS_SEPARATION_H__