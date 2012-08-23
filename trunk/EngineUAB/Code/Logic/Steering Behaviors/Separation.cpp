#include "Separation.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

#include "PhysicsManager.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"

#include "Math\MathUtils.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CSeparation::CSeparation( void )
	: CSteering				( ::separation )
	, m_UserDatas			( NULL )
	, m_SeparationUpdated	( false )
{
}

CSeparation::~CSeparation(void)
{
	m_UserDatas.clear();
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

//Vect3f CSeparation::CalculateSteering( CSteeringEntity *_pEntity )	
//{
//	Vect3f l_SeparationForce;
//	l_SeparationForce.SetZero();
//	
//	if ( m_SeparationUpdated )
//	{
//		m_SeparationUpdated = false;
//
//		for ( unsigned int i = 0; i < m_UserDatas.size(); ++i )
//		{
//			if ( m_UserDatas[i]->GetName() != _pEntity->GetName() ) 
//			{
//				Vect3f l_ToAgent = _pEntity->GetPosition() - m_UserDatas[i]->GetController()->GetPosition();
//
//				// Escalo la fuerza inversamente proporcional a la distancia de los agentes respecto a sus vecinos
//				l_SeparationForce += (l_ToAgent/l_ToAgent.Length()); 
//			}
//		}		
//	}
//	else
//	{
//		LOGGER->AddNewLog( ELL_ERROR, "Debe llamar primero al método UpdateParameters" );
//	}
//
//	// Retorno la fuerza
//    return (l_SeparationForce);   
//}

Vect3f CSeparation::CalculateSteering( CSteeringEntity *_pEntity )	
{
	Vect3f l_SeparationForce;
	l_SeparationForce.SetZero();
	
	Vect3f l_Strength;
	l_Strength.SetZero();
	
	if ( m_SeparationUpdated )
	{
		m_SeparationUpdated = false;

		for ( unsigned int i = 0; i < m_UserDatas.size(); ++i )
		{
			if ( m_UserDatas[i]->GetName() != _pEntity->GetName() ) 
			{
				Vect3f l_ToAgent = _pEntity->GetPosition() - m_UserDatas[i]->GetController()->GetPosition();
				float l_Distance =	l_ToAgent.Length();

				if ( l_Distance == 0 ) 
				{
					LOGGER->AddNewLog(ELL_INFORMATION, "eror");
				}
				
				// 0) Mètodo lineal: Escalo la fuerza inversamente proporcional a la distancia de los agentes respecto a sus vecinos. 
				// l_SeparationForce += (l_ToAgent/l_ToAgent.Length()); 
				
				// 1) Mètodo lineal: Escalo la fuerza inversamente proporcional a la distancia de los agentes respecto a sus vecinos. 
				// l_Strength = _pEntity->GetMaxAcceleration() * ( CORE->GetSteeringBehaviourSettingsManager()->GetNeightbourRadius() - l_Distance )/CORE->GetSteeringBehaviourSettingsManager()->GetNeightbourRadius();

				// 2) Mètodo de la ley del cuadrado inverso 
				if ( l_Distance < CORE->GetSteeringBehaviourSettingsManager()->GetNeightbourRadius() ) 
				{
					float l = CORE->GetSteeringBehaviourSettingsManager()->GetSeparationDecayCoefficient();
					float m = l_Distance * l_Distance;
					l_Strength = mathUtils::Min( l / m, _pEntity->GetMaxAcceleration() );
					//l_Strength = mathUtils::Min( CORE->GetSteeringBehaviourSettingsManager()->GetSeparationDecayCoefficient()/ (l_Distance * l_Distance), _pEntity->GetMaxAcceleration() );

					// Añado la aceleración
					l_ToAgent.Normalize();
					l_SeparationForce += l_Strength * l_ToAgent;
				}
			}
		}		
	}
	else
	{
		LOGGER->AddNewLog( ELL_ERROR, "Debe llamar primero al método UpdateParameters" );
	}

	// Retorno la fuerza
    return (l_SeparationForce);   
}
 

//Vect3f CSeparation::CalculateSteering( CSteeringEntity *_pEntity )	
//{
//	Vect3f l_SeparationForce;
//	l_SeparationForce.SetZero();
//	float l_Strength = 0.f;
//
//	if ( m_Target != NULL )
//    {
//		// Analizamos sólo los obstáculos dentro del alcance
//		SCollisionInfo l_Info; 
//		CPhysicUserData * l_pUserData = ThrowCollisionRay( _pEntity, l_Info, m_DetectionRayLength );
//
//		// Sabemos que no és ni el player ni él mismo y debemos calcular otra posición
//		if ( l_pUserData != NULL ) 
//		{
//			//calculate the distance between the positions of the entities
//			CPhysicActor * l_Actor			 = l_pUserData->GetActor();
//			CPhysicController * l_Controller = l_pUserData->GetController();
//
//			Vect3f l_Pos;
//			float  l_Radius = l_pUserData->GetRadius();
//
//			if ( l_Actor )
//			{
//				l_Pos = l_Actor->GetPosition();
//			}
//			else if ( l_Controller )
//			{
//				l_Pos = l_Controller->GetPosition();
//			}
//			else
//				return NULL;
//
//			std::string l_NameController = l_pUserData->GetName();
//			std::string l_NameEntity = _pEntity->GetName();
//
//		
//			Vect3f l_Direction = GetTarget() - _pEntity->GetPosition();
//			float l_Distance = l_Direction.Length();
//			
//			if ( l_Distance < m_DetectionRayLength )
//			{
//				// Calcula la fuerza de repulsion
//				l_Strength = mathUtils::Min( m_DecayCoefficient * l_Distance * l_Distance, _pEntity->GetMaxSpeed() );   //maxAcceleration )
//			}
//			// Añadimos la aceleración
//			l_Direction.Normalize();
//			l_SeparationForce = l_Strength * l_Direction;
//
//			//for ( unsigned int a=0; a< neighbors.size(); ++a )
//			//{
//			//	//make sure this agent isn't included in the calculations and that
//			//	//the agent being examined is close enough. ***also make sure it doesn't
//			//	//include the evade target ***
//			//	if((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged() && (neighbors[a] != m_pTargetAgent1))
//			//	{
//			//		Vector2D ToAgent = m_pVehicle->Pos() - neighbors[a]->Pos();
//
//			//		//scale the force inversely proportional to the agents distance  
//			//		//from its neighbor.
//			//		l_SeparationForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
//			//	}
//			//}
//		}
//		// Retorno la dirección a dirigirse siempre a máxima velocidad
//        return (l_SeparationForce);   //- _pEntity->GetVelocity()); Aquesta resta s'aplicaria només si volem sumar la part del vector velocitat que genera respecte la velocitat ja existent. 
//		
//    }
//    else 
//    {
//		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Seek");
//		return NULL;
//    }
//}

//
//CPhysicUserData* CSeparation::ThrowCollisionRay( CSteeringEntity *_pEntity, SCollisionInfo& _Info, float _MaxDistance )
//{
//	SCollisionInfo s;
//	
//	uint32 l_Mask = 1 << ECG_PLAYER;
//	l_Mask |= 1 << ECG_ENEMY;
//	l_Mask |= 1 << ECG_DYNAMIC_OBJECTS;
//	//l_Mask |= 1 << ECG_ESCENE;
//
//	Vect3f l_Direction = GetTarget() - _pEntity->GetPosition(); //+ _pEntity->GetFront();
//	//Vect3f l_Direction = _pEntity->GetPosition() + _pEntity->GetFront();
//	l_Direction.Normalize();
//
//	//Vect3f l_Pos = _pEntity->GetPosition() + _pEntity->GetBoundingRadius() + _pEntity->GetFront()/2 ;
//	Vect3f l_Position = _pEntity->GetPosition();
//	l_Position.x += _pEntity->GetFront().x * 2;
//	l_Position.y += _pEntity->GetHeight();
//	l_Position.z += _pEntity->GetFront().z * 2;
//
//	CPhysicUserData * l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_Position, l_Direction, l_Mask , s, _MaxDistance );
//	if ( l_Data ) 
//	{
//		// Si encuentro un controller y es el objetivo (el player) retorno no colisión. Lo mismo si no es controler que deberíamos tratar el actor
//		// Solo para testeo
//		if ( l_Data->GetController() )
//		{
//			if ( l_Data->GetController()->GetPosition().x == GetTarget().x  &&  l_Data->GetController()->GetPosition().z == GetTarget().z )
//			{
//				LOGGER->AddNewLog( ELL_INFORMATION, "Colisió d'enemic %s amb el player: %s", _pEntity->GetName().c_str(), l_Data->GetName().c_str());
//				return NULL;
//			}
//		}
//		else
//		{
//			// TODO: Caso de ser un CPHYSICACTOR
//			return NULL;
//		}
//
//		// Miro que no sea él mismo
//		std::string l_Name = l_Data->GetName().c_str();
//		if ( l_Name.compare( _pEntity->GetName() ) != 0 ) 
//		{
//			//LOGGER->AddNewLog( ELL_INFORMATION, "Colisió amb : %s", l_Data->GetName().c_str());
//			return l_Data;
//		}
//		else
//		{
//			LOGGER->AddNewLog( ELL_INFORMATION, "Colisiona %s amb ell mateix", l_Data->GetName().c_str());
//			return NULL;
//		}
//	}
//	
//	CHECKED_DELETE(l_Data);
//	return NULL;
//}

void CSeparation::UpdateParameters ( const std::vector<CPhysicUserData*> & _UserDatas )
{
	/*m_UserDatas.clear();
	m_UserDatas			= _UserDatas;*/
	m_SeparationUpdated = true;
}
