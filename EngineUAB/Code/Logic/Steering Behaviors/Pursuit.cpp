#include "Pursuit.h"
#include "Arrive.h"
#include "Seek.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CPursuit::CPursuit( void )
	: CSteering			( ::pursuit )
	//, m_Target			( NULL )
	, m_EvaderUpdated	( false )
	, m_pArrive			( NULL )
	, m_pEvader			( NULL )
{
	m_Deceleration		   = CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelaration();
	m_DecelerationDistance = CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelarationDistance();
	m_pArrive	= new CArrive( m_Deceleration, m_DecelerationDistance );
	m_pSeek		= new CSeek();
}

CPursuit::CPursuit( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance )
	: CSteering			( ::pursuit )
	//, m_Target			( NULL )
	, m_EvaderUpdated	( false )
	, m_pArrive			( NULL )
	, m_pEvader			( NULL )
{
	m_Deceleration		   = _Deceleration;
	m_DecelerationDistance = _DecelerationBeginsAtDistance;

	m_pArrive	= new CArrive( _Deceleration, _DecelerationBeginsAtDistance );
	m_pSeek		= new CSeek();
}


CPursuit::~CPursuit(void)
{
	CHECKED_DELETE( m_pArrive );
	CHECKED_DELETE( m_pSeek );
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CPursuit::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (!m_Target.IsZero())
    {
		if (m_EvaderUpdated)
        {
            m_EvaderUpdated = false;

			// Vector al objetivo
            Vect3f l_ToEvader = m_Target - _pEntity->GetPosition();

			// Comprobamos si el enemigo ya està encarado al player, en ese caso solo vamos a por él directamente
			float l_RelativeHeading = _pEntity->GetHeading().Dot(m_pEvader->GetHeading());
			float l_RelativeHeading0 = mathUtils::ACos( _pEntity->GetHeading().Dot(m_pEvader->GetHeading()) );
			float l_RelativeHeadingGraus = mathUtils::Rad2Deg( l_RelativeHeading );
			float l_RelativeHeading1 = l_ToEvader.Dot( _pEntity->GetHeading());
			float l_RelativeHeading2 = mathUtils::ACos( l_ToEvader.Dot( _pEntity->GetHeading()) );
			float l_RelativeHeadingGraus2 = mathUtils::Rad2Deg( l_RelativeHeading2 );
			//if ( ( l_ToEvader.Dot( _pEntity->GetHeading()) > 0) && ( l_RelativeHeading < -0.95 ) )  //acos(0.95)=18 degs
			if ( ( l_ToEvader.Dot( _pEntity->GetHeading()) > 0) && ( l_RelativeHeading < -0.95 ) )  //acos(0.95)=18 degs
			{
				m_pSeek->SetTarget( m_pEvader->GetPosition() );
				
				return m_pSeek->CalculateSteering( _pEntity );
			}

			// Si no está en linia al objetivo intentamos predecir donde irá el objetivo

			// el lookahead time es proporcional a la distancia entre el objetivo y el perseguidor y
			// inversamente proporcional a la suma de las velocidades de los agentes
			float l_LookAheadTime = l_ToEvader.Length() / ( _pEntity->GetMaxSpeed() + m_pEvader->GetSpeed() );

			// Ahora buscamos la futura posición predecida del objetivo
			/*m_pArrive->SetDeceleration( m_Deceleration );
			m_pArrive->SetDecelerationDistance( m_DecelerationDistance );
            m_pArrive->SetTarget( m_pEvader->GetPosition() + m_pEvader->GetVelocity() * l_LookAheadTime );

			return m_pArrive->CalculateSteering(_pEntity);*/

			Vect3f l_PredictePosition = m_pEvader->GetPosition() + m_pEvader->GetVelocity() * l_LookAheadTime;
			m_pSeek->SetTarget(l_PredictePosition);
			return m_pSeek->CalculateSteering(_pEntity);
        }
		else
		{
			LOGGER->AddNewLog( ELL_ERROR, "Debe llamar primero al método UpdateEvaderEntity" );
		}
	}
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo pursuit" );
    }

	return Vect3f(0,0,0);
}
   
 void CPursuit::UpdateEvaderEntity( CSteeringEntity *_pEvader ) 
{
	m_pEvader		= _pEvader;
	m_EvaderUpdated = true;
}