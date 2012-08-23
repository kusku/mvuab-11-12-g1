#include "OffsetPursuit.h"
#include "Arrive.h"
#include "Seek.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

#include "Math\Matrix33.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

COffsetPursuit::COffsetPursuit( void )
	: CSteering			( ::offset_pursuit )
	, m_pArrive			( NULL )
	, m_LeaderUpdated	( false )
{
	m_Deceleration		   = CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelaration();
	m_DecelerationDistance = CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelarationDistance();
	m_pArrive			   = new CArrive( m_Deceleration, m_DecelerationDistance );
}

COffsetPursuit::COffsetPursuit( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance )
	: CSteering			( ::offset_pursuit )
	, m_pArrive			( NULL )
	, m_LeaderUpdated	( false )
{
	m_Deceleration		   = _Deceleration;
	m_DecelerationDistance = _DecelerationBeginsAtDistance;

	m_pArrive = new CArrive( _Deceleration, _DecelerationBeginsAtDistance );
}


COffsetPursuit::~COffsetPursuit(void)
{
	CHECKED_DELETE( m_pArrive );
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f COffsetPursuit::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
		if (m_LeaderUpdated)
        {
            m_LeaderUpdated = false;

			Vect3f l_WorldOffsetPos = PointToWorldSpace( m_Offset, m_pLeader->GetHeading(), m_pLeader->GetSide(), m_pLeader->GetPosition() );

			// Vector al objetivo
            Vect3f l_ToOffset = m_Target - _pEntity->GetPosition();

			// el lookahead time es proporcional a la distancia entre el lider y el perseguidor y
			// inversamente proporcional a la suma de las velocidades de los agentes
			float l_LookAheadTime = l_ToOffset.Length() / ( _pEntity->GetMaxSpeed() / 2 + m_pLeader->GetSpeed() );

			// Ahora buscamos la futura posición predecida del objetivo
			m_pArrive->SetDeceleration( m_Deceleration );
			m_pArrive->SetDecelerationDistance( m_DecelerationDistance );
            m_pArrive->SetTarget( l_WorldOffsetPos + m_pLeader->GetVelocity() * l_LookAheadTime );

			return m_pArrive->CalculateSteering(_pEntity);
        }
		else
		{
			LOGGER->AddNewLog( ELL_ERROR, "Debe llamar primero al método UpdateLeaderEntity" );
			return NULL;
		}
	}
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo pursuit" );
		return NULL;
    }
}
   
 void COffsetPursuit::UpdateLeaderEntity( CSteeringEntity *_pLeader, eDeceleration _Deceleration, float _Offset )
{
	m_pLeader		= _pLeader;
	m_LeaderUpdated = true;
	m_Deceleration	= _Deceleration;
	m_Offset		= _Offset;
}


//--------------------- PointToWorldSpace --------------------------------
//
//  Transforms a point from the agent's local space into world space
//------------------------------------------------------------------------
inline Vect3f PointToWorldSpace( const Vect3f &_Point, const Vect3f &_AgentHeading, const Vect3f &_AgentSide, const Vect3f &_AgentPosition )
{
	//make a copy of the point
	Vect3f l_TransPoint = _Point;
	
	//create a transformation matrix
	//Mat44f l_MatTransform;
	//l_MatTransform.SetIdentity();

	////rotate
	//l_MatTransform.AlignXZ(_AgentHeading, _AgentSide);
	//
	////and translate
	//l_MatTransform.Translate(_AgentPosition);
	//
	////now transform the vertices
	//l_MatTransform.TransformVector(l_TransPoint);

	return l_TransPoint;
}
