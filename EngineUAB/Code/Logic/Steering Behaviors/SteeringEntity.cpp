#include "SteeringEntity.h"
#include "SteeringBehaviorsSeetingsManager.h"

#include "Scripting\ScriptManager.h"
#include "Math\Vector3.h"
#include "Base.h"
#include "Core.h"

#include "PhysicsManager.h"
#include "PhysicUserData.h"
#include "PhysicController.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

CSteeringEntity::CSteeringEntity( void )
	: m_pController			( NULL ) 
	, m_pHeadingSmoother	( NULL )
	, m_SmoothingOn			( true )
	, m_SmoothedHeading		( Vect3f(0,0,0) )
{
	// colocamos el smoother
	m_pHeadingSmoother = new CSmoother<Vect3f>(10, Vect3f(0.0, 0.0, 0.0)); 
}


CSteeringEntity::~CSteeringEntity( void )
{
	m_pController = NULL;
	CHECKED_DELETE( m_pHeadingSmoother );
}


const Vect3f CSteeringEntity::GetInitialPositionToThrowRay( void ) const
{
	Vect3f l_Pos; 
	Vect3f l_Front;
	//float l_Radi = CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerLength();
	float l_Radi = m_BoundingRadius;
	
	l_Pos = m_Position; 
	l_Front = GetFront();
	l_Front.Normalize();
	/*l_Pos.x = l_Pos.x + l_Front.x * l_Radi;
	l_Pos.z = l_Pos.z + l_Front.z * l_Radi;
	l_Pos.y += m_Height;*/

	l_Pos = Vect3f ( l_Pos.x + l_Front.x * l_Radi , l_Pos.y + m_Height, l_Pos.z + l_Front.z * l_Radi );
	/*v = Vect3f ( (*l_It)->GetPosition().x + (*l_It)->GetFront().x, (*l_It)->GetPosition().y + (*l_It)->GetProperties()->GetHeightController(), (*l_It)->GetPosition().z + (*l_It)->GetFront().z);
	l_RM->DrawLine( Vect3f ( (*l_It)->GetPosition().x, (*l_It)->GetPosition().y + (*l_It)->GetProperties()->GetHeightController() , (*l_It)->GetPosition().z ), v );*/

	return l_Pos;
}

const Vect3f CSteeringEntity::GetFinalPositionToThrowRay( float _DegresOfRotation ) const
{
	/*SCollisionInfo sInfo;
	uint32 l_Mask;
	l_Mask |= 1 << ECG_ESCENE;*/

	float l_ObstacleDistanceRay = CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerLength();
	Vect3f l_Front  = GetFront(); 
	if ( _DegresOfRotation != 0 ) 
		l_Front.RotateY(mathUtils::Deg2Rad( 360 - _DegresOfRotation));

	Vect3f l_OriginalFront = l_Front;
	l_Front.Normalize();
	
	Vect3f l_InitialPosition = GetInitialPositionToThrowRay();
	Vect3f l_FinalPosition; 
	Vect3f l_DirNormal; 
	
	// Lanzo un rayo para interceptar con el terreno y me de la altura del terreno a esa distancia de colision
	//CPhysicUserData * l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_InitialPosition, l_Front, l_Mask, sInfo );

	//// Si ha colisionado con el suelo es que seguramente hay una pendiente
	//if ( l_Data ) 
	//{
	//	sInfo.m_CollisionPoint.y += m_Height;
	//	l_DirNormal = sInfo.m_CollisionPoint - l_InitialPosition;
	//	l_DirNormal.Normalize();
	//	l_FinalPosition =  + l_DirNormal * l_ObstacleDistanceRay;
	//}
	//else
	//{
		// Compruebo si estamos en bajada
		//l_Front = l_OriginalFront;
		//float  l_DifY = m_PreviousPosition.y - m_Position.y;
		//Vect3f l_Direction = m_PreviousPosition - m_Position;
		//float l_Angle = l_Front.Dot( l_Direction );
		//
		//// Es menor de 90º
		//if ( l_Angle > 0.f )  // &&  ( l_DifY > m_Height ) )
		//{
		//	l_Front = m_Position + l_OriginalFront;
		//	//l_Front.y = m_Position.y + 
		//	//l_DirNormal = m
		//	//// Lanzo otro rayo hacia el suelo
		//	//CPhysicUserData * l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_InitialPosition, l_Front, l_Mask, sInfo );

		//}
		//// Es mayor de 90º
		//else if (  l_Angle > 0.f ) 
		//{
		//	l_Front = m_Position + l_OriginalFront;
		//	
		// 
		//}
		//// Caso en que no colisiona devuelvo un punto final de rayo horizontal
		//else
		//{
			l_FinalPosition  = Vect3f ( l_InitialPosition.x + l_Front.x * l_ObstacleDistanceRay, l_InitialPosition.y, l_InitialPosition.z + l_Front.z * l_ObstacleDistanceRay);
		//}
//	}

	//l_FinalPosition  = Vect3f ( l_InitialPosition.x + l_Front.x * l_ObstacleDistanceRay, l_InitialPosition.y, l_InitialPosition.z + l_Front.z * l_ObstacleDistanceRay);
	return l_FinalPosition;
}