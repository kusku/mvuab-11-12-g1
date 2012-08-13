#include <algorithm>
#include <vector>

#include "SteeringBehaviorsSeetingsManager.h"
#include "SteeringBehaviors.h"
#include "SteeringBehaviorsDefs.h"
#include "SteeringEntity.h"

#include "Seek.h"
#include "Flee.h"
#include "Pursuit.h"
//#include "Evade.h"
//#include "OffsetPursuit.h"
#include "Arrive.h"
//#include "Wander.h"
//#include "CollisionAvoidance.h"
//#include "ObstacleWallAvoidance.h"
// --- Para Flocking Behaviur ---- 
//#include "Separation.h"
//#include "Alignment.h"
//#include "Cohesion.h"
// -------------------------------

//#include "PhysicsManager.h"
//#include "PhysicUserData.h"
//#include "PhysicsDefs.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CSteeringBehaviors::CSteeringBehaviors( float _MaxForce )
	: m_SteeringForce			( Vect3f(0.f, 0.f, 0.f) )
	, m_pSeek					( NULL )
	//, m_pFlee					( NULL )
	, m_pPursuit				( NULL )
	, m_pArrive					( NULL )
	//, m_pCollisionAvoidance		( NULL )
	//, m_pObstacleWallAvoidance	( NULL )
	//, m_pSeparation				( NULL )
	//, m_pAlignment				( NULL )
	//, m_pCohesion				( NULL )
	, m_MaxForce				( _MaxForce )
	, m_SummingMethod			( ::prioritized )
{
	m_Behaviors = new std::list<eBehaviorType>();

	Initialize();
}

CSteeringBehaviors::~CSteeringBehaviors(void)
{

}


// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CSteeringBehaviors::Initialize( void )
{
	m_Behaviors->clear();
	
	m_WeightWallAvoidance		= 0.82f;
	m_WeightObstacleAvoidance	= 0.82f;
	m_WeightSeek				= 0.22f;
	m_WeightEvade				= 0.22f;
	m_WeightFlee				= 0.22f;
	m_WeightArrive				= 0.22f;
	m_WeightPursuit				= 0.22f;
	m_WeightWander				= 0.22f;
	m_WeightSeparation			= 0.22f;

}

void CSteeringBehaviors::Destroy( void )
{
	m_Behaviors->clear();

	// Ojo, les classes dels comportaments les creo desde lua!! Així que no cal eliminar-les aquí.
}

const Vect3f& CSteeringBehaviors::Update( float _ElapsedTime, CSteeringEntity * _pEntity)  
{
	// Reseteamos el vector de fuerza direccional
    m_SteeringForce.SetZero();
	
	// Almaceno la entidad tratada
	m_pEntity = _pEntity;

	// Segun el método asumido calcularemos los valores. Por defecto devolveremos Zero
	switch (m_SummingMethod)
	{
		case weighted_average:
    		m_SteeringForce = CalculateWeightedSum(); break;

		case prioritized:
				m_SteeringForce = CalculatePrioritized(); break;

		case dithered:
				m_SteeringForce = CalculateDithered();break;

		default:m_SteeringForce = Vect3f(0,0,0); 

	}//end switch

    // Devolvemos la fuerza producida por todos los comportamientos 
    return m_SteeringForce;
}

const Vect3f& CSteeringBehaviors::CalculateWeightedSum( void )  
{
	return m_SteeringForce;
}

//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool CSteeringBehaviors::AccumulateForce( Vect3f &_CurrentSteeringForce, Vect3f _ForceToAdd )
{
	// Calcula la longitud de la fuerza steering calculada hasta ahora
	float l_MagnitudeSoFar = _CurrentSteeringForce.Length();

	// Calcula cuanta fuerza steering queda pendiente para ser usada por la entidad
	float l_MagnitudeRemaining = m_pEntity->GetMaxForce() - l_MagnitudeSoFar;

	// Retornamos false si no hay más fuerza disponible, si ya estamos en el máximo permitido
	if ( l_MagnitudeRemaining <= 0.0 ) return false;

	// Calculamos la magnitud de la fuerza que se desea añadir 
	float l_MagnitudeToAdd = _ForceToAdd.Length();
  
	// Si la magnitud de la suma de _ForceToAdd y la actual no excede 
	// de la maxima fuerza disponible para esta entidad añadimos esta. 
	// De lo contrario añadimos como mucho la cantidad de _ForceToAdd 
	// que sea posible sin pasar del máximo.
	if ( l_MagnitudeToAdd < l_MagnitudeRemaining)
	{
		_CurrentSteeringForce += _ForceToAdd;
	}
	else
	{
		// se añade toda la fuerza steering disponible
		_CurrentSteeringForce += _ForceToAdd.Normalize() * l_MagnitudeRemaining; 
	}

	return true;
}

//---------------------- CalculatePrioritized ----------------------------
//
//  Metodo que llama cada steering behavior activo en orden de prioridad 
//  y acumula las fuerzas hasta la màxima magnitud de steering force 
//  posible, momento en el que la función retorna la steering force 
//  acumulada a ese punto
//------------------------------------------------------------------------
const Vect3f& CSteeringBehaviors::CalculatePrioritized( void )  
{
	Vect3f l_Force;
	l_Force.SetZero();
	
	//if ( HasBehavior(::obstacle_wall_avoidance) && On(::obstacle_wall_avoidance))
	//{
	//	l_Force = m_pObstacleWallAvoidance->CalculateSteering(m_pEntity) * m_WeightWallAvoidance;

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	//if ( HasBehavior(::collision_avoidance) && On(::collision_avoidance) )
	//{
	//	l_Force = m_pCollisionAvoidance->CalculateSteering(m_pEntity) * m_WeightObstacleAvoidance;
	//	
	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	//if ( HasBehavior(::evade) && On(::evade) )
	//{	
	//	l_Force = m_pEvade->CalculateSteering(m_pEntity) * m_WeightEvade;

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	//if ( HasBehavior(::flee) && On(::flee) )
	//{
	//	l_Force = m_pFlee->CalculateSteering(m_pEntity) * m_WeightFlee;

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}
	
	// ---  tratamiento grupal ---
	// En caso de comportamiento grupal buscamos los agentes cercanos dentro del radio especificado
	//if (On(::separation) || On(::alignment) || On(::cohesion))
 //   {
	//	if (m_pSeparation== NULL )
	//	{
	//		float l = 0.f;
	//	}
	//	else 
	//	{
	//		float l = 0.f;
	//	}
	//	if ( m_UserDatas.size() == 0 )
	//	{
	//		float l = 0.f;
	//	}
	//	else 
	//	{
	//		float l = 0.f;
	//	}
	//	if (m_pEntity== NULL )
	//	{
	//		float l = 0.f;
	//	}
	//	else 
	//	{
	//		float l = 0.f;
	//	}

	//	//CalculateNeighbors( m_pEntity->GetPosition(), CORE->GetSteeringBehaviourSettingsManager()->GetNeightbourRadius() );

	//	if (m_pSeparation== NULL )
	//	{
	//		float l = 0.f;
	//	}
	//	else 
	//	{
	//		float l = 0.f;
	//	}
	//	if ( m_UserDatas.size() == 0 )
	//	{
	//		float l = 0.f;
	//	}
	//	else 
	//	{
	//		float l = 0.f;
	//	}
	//	if (m_pEntity== NULL )
	//	{
	//		float l = 0.f;
	//	}
	//	else 
	//	{
	//		float l = 0.f;
	//	}
 //   }

	//if ( HasBehavior(::separation) && On(::separation) )
 //   {
	//	//m_pSeparation->UpdateParameters(m_UserDatas);
	//	//l_Force = m_pSeparation->CalculateSteering(m_pEntity) * m_WeightSeparation;
	//	l_Force = Vect3f(0,0,0);

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
 //   }

	//if ( HasBehavior(::alignment) && On(::alignment) )
 //   {
	//	m_pAlignment->UpdateParameters(m_UserDatas);
	//	l_Force = m_pAlignment->CalculateSteering(m_pEntity) * m_WeightAlignment;

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
 //   }
	//
 //   if ( HasBehavior(::cohesion) && On(::cohesion) )
 //   {
	//	m_pCohesion->UpdateParameters(m_UserDatas);
	//	l_Force = m_pCohesion->CalculateSteering(m_pEntity) * m_WeightCohesion;

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
 //   }
	// ---  fin tratamiento grupal ---

	if ( HasBehavior(::seek) && On(::seek) )
	{
        l_Force = m_pSeek->CalculateSteering(m_pEntity) * m_WeightSeek;
		
		if (!AccumulateForce(m_SteeringForce, l_Force)) 
			return m_SteeringForce;
	}	
	
	if ( HasBehavior(::arrive) && On(::arrive) )
	{
		l_Force = m_pArrive->CalculateSteering(m_pEntity) * m_WeightArrive;

		if (!AccumulateForce(m_SteeringForce, l_Force)) 
			return m_SteeringForce;
	}	

	//if ( HasBehavior(::wander) && On(::wander) ) 
	//{
	//	l_Force = m_pWander->CalculateSteering(m_pEntity) * m_WeightWander;

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	if ( HasBehavior(::pursuit) && On(::pursuit) ) 
	{
		//assert(m_pTargetAgent1 && "pursuit target not assigned");

		//l_Force = Pursuit(m_pTargetAgent1) * m_WeightPursuit;
		l_Force = m_pPursuit->CalculateSteering(m_pEntity) * m_WeightPursuit;

		if (!AccumulateForce(m_SteeringForce, l_Force)) 
			return m_SteeringForce;
	}

	//if ( HasBehavior(::offset_pursuit) && On(::offset_pursuit) ) 
	//{
	//	/*assert (m_pTargetAgent1 && "pursuit target not assigned");
	//	assert (!m_vOffset.isZero() && "No offset assigned");*/

	//	l_Force =  m_pOffsetPursuit->CalculateSteering(m_pEntity) * m_WeightOffsetPursuit;
	//	//l_Force = m_pOffsetPursuit(m_pTargetAgent1, m_vOffset);

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	//if ( HasBehavior(::interpose) && On(::interpose) ) 
	//{
	//	/*assert (m_pTargetAgent1 && m_pTargetAgent2 && "Interpose agents not assigned");

	//	l_Force = m_pInterpose(m_pTargetAgent1, m_pTargetAgent2) * m_WeightInterpose;*/

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	//if ( HasBehavior(::hide) && On(::hide) ) 
	//{
	//	/*assert(m_pTargetAgent1 && "Hide target not assigned");

	//	l_Force = m_pHide(m_pTargetAgent1, m_pVehicle->World()->Obstacles()) * m_WeightHide;*/

	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}


	//if ( HasBehavior(::follow_path) && On(::follow_path) ) 
	//{
	//	//l_Force = m_pFollowPath->CalculateSteering(m_pEntity) * m_WeightFollowPath;
	//
	//	if (!AccumulateForce(m_SteeringForce, l_Force)) 
	//		return m_SteeringForce;
	//}

	return m_SteeringForce;
}

const Vect3f& CSteeringBehaviors::CalculateDithered( void )  
{
	return m_SteeringForce;
}

// -----------------------------------------
//				MÈTODES
// -----------------------------------------
 
void CSteeringBehaviors::AddBehavior( CSeek *_pSteering ) 
{
    if ( !HasBehavior(_pSteering->GetType() ) )
		m_Behaviors->push_back(_pSteering->GetType());

    m_pSeek = _pSteering;
}

//void CSteeringBehaviors::AddBehavior( CFlee *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) )
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    m_pFlee = _pSteering;
//}

void CSteeringBehaviors::AddBehavior( CPursuit *_pSteering ) 
{
    if ( !HasBehavior(_pSteering->GetType() ) )
		m_Behaviors->push_back(_pSteering->GetType());

    m_pPursuit = _pSteering;
}

//void CSteeringBehaviors::AddBehavior( CEvade *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) )
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    m_pEvade = _pSteering;
//}

void CSteeringBehaviors::AddBehavior( CArrive *_pSteering ) 
{
    if ( !HasBehavior(_pSteering->GetType() ) ) 
		m_Behaviors->push_back(_pSteering->GetType());

    m_pArrive = _pSteering;
}

//void CSteeringBehaviors::AddBehavior( CWander *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) )
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    m_pWander = _pSteering;
//}
//
//void CSteeringBehaviors::AddBehavior( CCollisionAvoidance *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) ) 
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    m_pCollisionAvoidance = _pSteering;
//}
//
//void CSteeringBehaviors::AddBehavior( CObstacleWallAvoidance *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) ) 
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    m_pObstacleWallAvoidance = _pSteering;
//}
//
//void CSteeringBehaviors::AddBehavior( CSeparation *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) ) 
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    //m_pSeparation = _pSteering;
//}
//
//void CSteeringBehaviors::AddBehavior( CAlignment *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) ) 
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    //m_pAlignment = _pSteering;
//}
//
//void CSteeringBehaviors::AddBehavior( CCohesion *_pSteering ) 
//{
//    if ( !HasBehavior(_pSteering->GetType() ) ) 
//		m_Behaviors->push_back(_pSteering->GetType());
//
//    //m_pCohesion = _pSteering;
//}

//void CSteeringBehaviors::ClearNeighbors( void )
//{
//	m_UserDatas.clear();
//}
//
//void CSteeringBehaviors::CalculateNeighbors( const Vect3f & _Position, float _ViewDistance )
//{
//	int l_Mask = 1 << ECG_ENEMY;
//
//	// Limpiamos la lista anterior
//	ClearNeighbors();
//
//	// Calculamos los vecinos a una posición 
//	CORE->GetPhysicsManager()->OverlapSphereActor( _ViewDistance, _Position, m_UserDatas, l_Mask );
//
//	//if ( m_UserDatas.size() > 0 )
//	//{
//	//	LOGGER->AddNewLog( ELL_INFORMATION, "He trobat veins" );
//	//	return;
//	//}
//}

// -----------------------------------------
//				PROPERTIES
// -----------------------------------------

int CSteeringBehaviors::HasBehavior( eBehaviorType _Type )  
{
	tBehaviourType::iterator l_Found = m_Behaviors->begin();
	
	int l_NumBehavioursFound = 0;

	while (true) 
	{
		l_Found = std::find ( l_Found, m_Behaviors->end(), _Type );
		if (l_Found == m_Behaviors->end()) break;

		l_NumBehavioursFound++;
		l_Found++;
	}
    
	return l_NumBehavioursFound;
}

CSeek* CSteeringBehaviors::GetSeek( void ) 
{
	if ( m_pSeek == NULL && HasBehavior(::seek) )
	{
			m_pSeek = new CSeek();
    }

	return m_pSeek;
}

//CFlee * CSteeringBehaviors::GetFlee( void ) 
//{
//	if ( m_pFlee == NULL && HasBehavior(::flee) )
//	{
//			m_pFlee = new CFlee(0.f);
//    }
//
//	return m_pFlee;
//}

CPursuit* CSteeringBehaviors::GetPursuit( void ) 
{
	if ( m_pPursuit == NULL && HasBehavior(::pursuit) )
	{
			m_pPursuit = new CPursuit( ::normal, 10.f );
    }

	return m_pPursuit;
}

//CEvade* CSteeringBehaviors::GetEvade( void ) 
//{
//	if ( m_pEvade == NULL && HasBehavior(::evade) )
//	{
//			m_pEvade = new CEvade( 0.f );
//    }
//
//	return m_pEvade;
//}

CArrive* CSteeringBehaviors::GetArrive( void )
{
	if ( m_pArrive == NULL && HasBehavior(::arrive) )
	{
			m_pArrive = new CArrive( CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelaration() , CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelarationDistance() );
    }

	return m_pArrive;
}

//CWander* CSteeringBehaviors::GetWander( void )
//{
//	if ( m_pWander == NULL && HasBehavior(::wander) )
//	{
//			CSteeringBehaviorSeetingsManager * l_Manager =  CORE->GetSteeringBehaviourSettingsManager();
//			m_pWander = new CWander( l_Manager->GetWanderRefreshRate(),  l_Manager->GetWanderDistance(), l_Manager->GetWanderRadius(), l_Manager->GetArriveDecelaration() , l_Manager->GetArriveDecelarationDistance() );
//			l_Manager = NULL;
//    }
//
//	return m_pWander;
//}
//
//CCollisionAvoidance* CSteeringBehaviors::GetCollisionAvoidance( void )
//{
//	if ( m_pCollisionAvoidance == NULL && HasBehavior(::collision_avoidance) )
//	{
//			m_pCollisionAvoidance = new CCollisionAvoidance( CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerLength() );
//    }
//
//	return m_pCollisionAvoidance;
//}
//
//CObstacleWallAvoidance* CSteeringBehaviors::GetObstacleWallAvoidance( void )
//{
//	if ( m_pObstacleWallAvoidance == NULL && HasBehavior(::obstacle_wall_avoidance) )
//	{
//			m_pObstacleWallAvoidance = new CObstacleWallAvoidance( CORE->GetSteeringBehaviourSettingsManager()->GetObstacleWallDetectionFeelerLength() );
//    }
//
//	return m_pObstacleWallAvoidance;
//}

//CSeparation * CSteeringBehaviors::GetSeparation( void )
//{
//	if ( m_pSeparation == NULL && HasBehavior(::separation) )
//	{
//		m_pSeparation = new CSeparation();
//    }
//
//	return m_pSeparation;
//}
//
//CAlignment * CSteeringBehaviors::GetAlignment( void )
//{
//	if ( m_pAlignment == NULL && HasBehavior(::alignment) )
//	{
//		m_pAlignment = new CAlignment();
//    }
//
//	return m_pAlignment;
//}
//
//CCohesion * CSteeringBehaviors::GetCohesion( void )
//{
//	if ( m_pCohesion == NULL && HasBehavior(::cohesion) )
//	{
//		m_pCohesion = new CCohesion();
//    }
//
//	return m_pCohesion;
//}
