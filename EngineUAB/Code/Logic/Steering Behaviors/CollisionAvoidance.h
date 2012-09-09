#pragma once

#ifndef __CLASS_collision_avoidance_H__
#define __CLASS_collision_avoidance_H__


#include <vector>
#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"
#include "SteeringBehaviorsDefs.h"

#include "PhysicsDefs.h"

const float		BRAKING_WEIGHT = 0.2f;

//--- Foward Declarations ---
class CSteeringEntity;
class CBaseGameEntity;
class CCCharacterManager;
class CPhysicUserData;
class CEvade;
//---------------------------

class CCollisionAvoidance : public CSteering
{
public:
	


	//--- Init and End protocols------------------------------------------
							CCollisionAvoidance				( float _PanicDistance );
							CCollisionAvoidance				( float _PanicDistance, float _MinDetectionLength );
	virtual					~CCollisionAvoidance			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering				( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
	virtual	Vect3f			CalculateSteering1				( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
	virtual	Vect3f			CalculateSteering2				( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
	virtual	Vect3f			CalculateSteering3				( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
	void					CreateFeelers					( float _Angle );

	//---- Functions ---------------------------------------
private:	
	void					EnforceNonPenetrationConstraint	( CSteeringEntity *_Entity, CPhysicUserData * _pUserDataObstacle );
	CPhysicUserData*		ShootCollisionRay				( CSteeringEntity *_pEntity, const Vect3f &_Direction, SCollisionInfo& _Info, float _MaxDistance = 0xffffffff );

	inline Vect3f			GetObstaclePosition				( CPhysicUserData * _pUserData );
	inline Vect3f			GetObstacleVelocity				( CPhysicUserData * _pUserData );

	inline const CSteeringEntity*	GetPursuer				( CSteeringEntity * m_pPursuer ) const;
	inline const void				SetPursuer				( CSteeringEntity * _pPursuer )				{ m_pPursuer = _pPursuer; }
	

	//---- Properties ( get & Set )---------------------------------------
	
	//---- Members -----------------------------------------------
private:
		CSteeringEntity*	m_pEntity;						// Entidad de la qual tratamos si colisiona
        	
		std::vector<Vect3f>	m_Feelers;						// 3 Vectores 3f que seran los rayos de intersección "Los bigotes del gato!"
		float	 			m_DetectionRayLength;			// Distancia de detección de obstaculos
		float				m_PanicDistance;				// Distancia de pànico para llamar al comportamiento de evasion

        float				m_NearestObstacle;
        Vect3f				m_NearestSteering;
        
		CEvade*				m_pEvade;
		CSteeringEntity*	m_pPursuer;
};

#endif __CLASS_collision_avoidance_H__