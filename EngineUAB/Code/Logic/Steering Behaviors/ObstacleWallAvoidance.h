#pragma once

#ifndef __CLASS_OBSTACLE_WALL_AVOIDANCE_H__
#define __CLASS_OBSTACLE_WALL_AVOIDANCE_H__


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
	virtual	Vect3f			CalculateSteering			( CSteeringEntity *_pEntity );		// M�todo que deber� ser sobreescrito en cada clase que herede de Steering
	void 					CreateFeelers				( float _Angle );					// Crea rayos para ver si la entidad colisiona con alguna pared dentro de ese �ngulo
	void					DrawRays					( void );
	
	//---- Functions ---------------------------------------
private:	
	const Vect3f			GetInitialPositionToThrowRay( void ) const;
	const Vect3f			GetDirectionRay				( float _Degrees ) const;

	//CPhysicUserData*		ThrowCollisionRays			( CSteeringEntity *_pEntity, SCollisionInfo& _Info, float _MaxDistance = 0xffffffff );

	//---- Properties ( get & Set )---------------------------------------
	
	//---- Members -----------------------------------------------
private:
		float	 			m_ObstacleWallDetectionFeelerLength;	// Distancia de detecci�n de los rayos
        std::vector<Vect3f>	m_Feelers;								// 3 Vectores 3f que seran los rayos de intersecci�n "Los bigotes del gato!"

		CSteeringEntity*	m_pEntity;								// Entidad de la qual tratamos si colisiona
};

#endif __CLASS_OBSTACLE_WALL_AVOIDANCE_H__
