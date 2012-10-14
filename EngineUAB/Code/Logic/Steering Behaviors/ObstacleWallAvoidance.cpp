#include "ObstacleWallAvoidance.h"
#include "StatesMachine\BaseGameEntity.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

//#include "Characters\CharacterManager.h"
//#include "Characters\Character.h"
#include "Utils\TemplatedVectorMapManager.h"
//#include "GameProcess.h"

#include "PhysicsManager.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"

#include "RenderManager.h"
#include "Fonts\FontManager.h"

#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CObstacleWallAvoidance::CObstacleWallAvoidance( float _DetectionRayLength )
	: CSteering								( ::obstacle_wall_avoidance )
	, m_ObstacleWallDetectionFeelerLength	( _DetectionRayLength )
{
}


CObstacleWallAvoidance::~CObstacleWallAvoidance(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CObstacleWallAvoidance::CalculateSteering( CSteeringEntity *_pEntity )	
{
	// Almacenamos la entidad tratada
	m_pEntity = _pEntity;

	// Creo los bigotitos...
	CreateFeelers(CORE->GetSteeringBehaviourSettingsManager()->GetObstacleWallDetectionFeelerAngle());

	SCollisionInfo sInfo;
	
	Vect3f l_SteeringForce;
	l_SteeringForce.SetZero();

	uint32 l_Mask = 1 << ECG_LIMITS;
	l_Mask = 1 << ECG_ESCENE;
	
	// Distancia al punto de intersección más proximo
	float l_DistanceToClosestIP = m_ObstacleWallDetectionFeelerLength;
	
	// Punto más proximo de colision
	Vect3f l_ClosestPoint;
	l_ClosestPoint.SetZero();

	// Vector normal de la superficie mas proxima
	Vect3f l_ClosestNormal;
	l_ClosestNormal.SetZero();

	for ( size_t flr = 0; flr < m_Feelers.size(); ++flr)
	{
		Vect3f l_InitialPosition = GetInitialPositionToThrowRay();
		
		// Siempre hay que pasar la dirección normalizada
		CPhysicUserData * l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_InitialPosition, m_Feelers[flr], l_Mask, sInfo, m_ObstacleWallDetectionFeelerLength );
		
		// Si ha colisionado con algo miramos si está más proximo
		if ( l_Data ) 
		{
			if ( sInfo.m_fDistance < l_DistanceToClosestIP )
			{
				l_DistanceToClosestIP = sInfo.m_fDistance;		// metemos la nueva mínima distancia de una entidad más pròxima
					
				l_ClosestPoint = sInfo.m_CollisionPoint;		// Guardamos el punto de colisión más pròximo encontrado
				
				l_ClosestNormal = sInfo.m_Normal;				// Guardamos la normal de la superficie interceptada
			}
		
			// Si encuentro colision de algun rayo con algun actor le añado la fuerza correspondiente
			// Calculo por qué distancia la posición proyectada del agente sobrepasará el límite
			Vect3f l_OverShoot = m_Feelers[flr] - l_ClosestPoint;

			// Creamos una fuerza en la dirección de la normal del límite, con una magnitud create a force in the direction of the wall normal, con una magnitud de sobrepasado
			l_SteeringForce += l_ClosestNormal * l_OverShoot.Length();	
		}
	}
	
	return l_SteeringForce;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void CObstacleWallAvoidance::CreateFeelers( float _Angle )
{
	//m_Feelers.push_back(GetDirectionRay(0.f));
	//m_Feelers.push_back(GetDirectionRay(30.f));
	//m_Feelers.push_back(GetDirectionRay(-30.f));

	m_Feelers.clear();

	float l_Increment = CORE->GetSteeringBehaviourSettingsManager()->GetObstacleWallDetectionFeelerAngleIncrement();
	for ( float i = -_Angle/2; i < _Angle/2; i += l_Increment ) 
	{
		m_Feelers.push_back(GetDirectionRay(i));
		/*Vect3f l_Front = m_pEntity->GetFront();
		l_Front.y = 0;
		l_Front.RotateY(mathUtils::Deg2Rad(i));
		m_Feelers.push_back(l_Front);*/
	}
}

void CObstacleWallAvoidance::DrawRays( Vect3f _Ray )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	l_RM->SetTransform(mat);

	Vect3f l_InitialPosition = m_pEntity->GetPosition();
	l_InitialPosition.y += m_pEntity->GetHeight()/2;
	Vect3f l_FinalPosition = _Ray.GetNormalized();
	l_FinalPosition += l_InitialPosition;
	l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colWHITE );
}

const Vect3f CObstacleWallAvoidance::GetInitialPositionToThrowRay( void ) const
{
	Vect3f l_Pos = m_pEntity->GetPosition();
	l_Pos.y += m_pEntity->GetHeight()/2;
	
	Vect3f l_Front;
	l_Front = m_pEntity->GetFront();
	l_Front.Normalize();
	
	l_Pos = Vect3f ( l_Pos.x + l_Front.x, l_Pos.y, l_Pos.z);
	
	return l_Pos;
}

const Vect3f CObstacleWallAvoidance::GetDirectionRay( float _DegresOfRotation ) const
{
	Vect3f l_Front	= m_pEntity->GetFront();
	l_Front.Normalize();
		
	if ( _DegresOfRotation != 0 ) 
		l_Front.RotateY(mathUtils::Deg2Rad(_DegresOfRotation));

	return (Vect3f(l_Front.x, 0.f, l_Front.z));
}

//CPhysicUserData* CObstacleWallAvoidance::ThrowCollisionRays( CSteeringEntity *_pEntity, SCollisionInfo& _Info, float _MaxDistance )
//{
//	Vect3f l_SteeringForce;
//	l_SteeringForce.SetZero();
//
//	uint32 l_Mask = 1 << ECG_LIMITS;
//	
//	// Distancia al punto de intersección más proximo
//	float l_DistanceToClosestIP = 0.f;
//	
//	// Punto más proximo de colision
//	Vect3f l_ClosestPoint;
//	l_ClosestPoint.SetZero();
//
//	// Vector normal de la superficie mas proxima
//	Vect3f l_ClosestNormal;
//	l_ClosestNormal.SetZero();
//
//	for ( int flr = 0; flr < m_Feelers.size(); ++flr)
//	{
//		// Siempre hay que pasar la dirección normalizada
//		CPhysicUserData * l_Data = CORE->GetPhysicsManager()->RaycastClosestActor( _pEntity->GetInitialPositionToThrowRay(), m_Feelers[flr].Normalize(), l_Mask, sInfo, _MaxDistance );
//		
//		// Si ha colisionado con algo miramos si está más proximo
//		if ( l_Data ) 
//		{
//			if ( sInfo.m_fDistance < l_DistanceToClosestIP )
//			{
//				l_DistanceToClosestIP = sInfo.m_fDistance;		// metemos la nueva mínima distancia de una entidad más pròxima
//					
//				l_ClosestPoint = sInfo.m_CollisionPoint;		// Guardamos el punto de colisión más pròximo encontrado
//				
//				l_ClosestNormal = sInfo.m_Normal;				// Guardamos la normal de la superficie interceptada
//			}
//		
//			// Si encuentro colision de algun rayo con algun actor le añado la fuerza correspondiente
//			// Calculo por qué distancia la posición proyectada del agente sobrepasará el límite
//			Vect3f l_OverShoot = m_Feelers[flr] - l_ClosestPoint;
//
//			// Creamos una fuerza en la dirección de la normal del límite, con una magnitud create a force in the direction of the wall normal, con una magnitud de sobrepasado
//			l_SteeringForce += l_ClosestNormal * l_OverShoot.Length();	
//		}
//	}
//	
//	//Vect3f l_Direction = GetTarget() - _pEntity->GetPosition(); //+ _pEntity->GetFront();
//	//l_Direction.Normalize();
//
//	////Vect3f l_Position = _pEntity->GetPosition() + _pEntity->GetBoundingRadius() + _pEntity->GetFront()/2 ;
//	//Vect3f l_Position = _pEntity->GetPosition();
//	//l_Position.x += _pEntity->GetBoundingRadius() * 2;
//	//l_Position.y += _pEntity->GetBoundingRadius() / 2:
//	//l_Position.z += _pEntity->GetBoundingRadius() * 2;
//
//	// El rayo frontal no funcionó o no col·lisionó. Lanzo otro en un lado del fustrum
//	//if ( !l_Data ) 
//	//{
//	//	Vect3f l_Front = _pEntity->GetFront();
//	//	l_Front.RotateY(mathUtils::Deg2Rad(30.f));
//	//	l_Dir = Vect3f ( GetTarget().x + l_Front.x + l_Front.x  , GetTarget().y + _pEntity->GetHeight(), GetTarget().z + l_Front.z + l_Front.z );
//	//	l_Dir.Normalize();
//	//	l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_Pos, l_Dir, l_Mask , s, _MaxDistance );
//
//	//	if ( l_Data )
//	//	{
//	//		std::string l_Name = l_Data->GetName().c_str();
//	//		if ( l_Name.compare(_pEntity->GetName()) != 0 ) 
//	//		{
//	//			LOGGER->AddNewLog( ELL_INFORMATION, "Colisió LATERAL 30º d'enemic %s amb el player: %s", _pEntity->GetName().c_str(), l_Data->GetName().c_str());
//	//		}
//	//	}
//	//}
//
//	//// El rayo frontal no funcionó o no col·lisionó. Lanzo otro en un lado del fustrum
//	//if ( !l_Data ) 
//	//{
//	//	Vect3f l_Front = _pEntity->GetFront();
//	//	l_Front.RotateY(mathUtils::Deg2Rad(30.f));
//	//	l_Dir = Vect3f ( GetTarget().x + l_Front.x + l_Front.x  , GetTarget().y + _pEntity->GetHeight(), GetTarget().z + l_Front.z + l_Front.z );
//	//	l_Dir.Normalize();
//	//	l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_Pos, l_Dir, l_Mask , s, _MaxDistance );
//
//	//	if ( l_Data )
//	//	{
//	//		std::string l_Name = l_Data->GetName().c_str();
//	//		if ( l_Name.compare(_pEntity->GetName()) != 0 ) 
//	//		{
//	//			LOGGER->AddNewLog( ELL_INFORMATION, "Colisió LATERAL -30º d'enemic %s amb el player: %s", _pEntity->GetName().c_str(), l_Data->GetName().c_str());
//	//		}
//	//	}
//	//}
//	
//	return l_SteeringForce;
//}



