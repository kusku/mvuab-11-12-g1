#pragma once

#ifndef __CLASS_obstacle_wall_avoidance_H__
#define __CLASS_obstacle_wall_avoidance_H__


#include <vector>
#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

#include "PhysicsDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
class CBaseGameEntity;
class CCCharacterManager;
class CPhysicUserData;
//---------------------------

class CObstacleWallAvoidance : public CSteering
{
public:
	


	//--- Init and End protocols------------------------------------------
							CObstacleWallAvoidance		( float _DetectionRayLength );
							//CObstacleWallAvoidance	( const std::vector<CBaseGameEntity> &Obstacles, float _MinDetectionBoxLength );
	virtual					~CObstacleWallAvoidance		( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering			( CSteeringEntity *_pEntity );		// Método que deberá ser sobreescrito en cada clase que herede de Steering
	void 					CreateFeelers				( void );							// Crea 3 rayos para ver si la entidad colisiona con alguna pared

	//---- Functions ---------------------------------------
private:	
	
	//CPhysicUserData*		ThrowCollisionRays			( CSteeringEntity *_pEntity, SCollisionInfo& _Info, float _MaxDistance = 0xffffffff );

	//---- Properties ( get & Set )---------------------------------------
	
	//---- Members -----------------------------------------------
private:
		float	 			m_ObstacleWallDetectionFeelerLength;	// Distancia de detección de los rayos
        std::vector<Vect3f>	m_Feelers;								// 3 Vectores 3f que seran los rayos de intersección "Los bigotes del gato!"

		CSteeringEntity*	m_pEntity;								// Entidad de la qual tratamos si colisiona
};

#endif __CLASS_obstacle_wall_avoidance_H__
