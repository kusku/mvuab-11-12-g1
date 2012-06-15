#include "Pursuit.h"
#include "Steering Behaviours\SteeringBehavioursDefs.h"
#include "Steering Behaviours\SteeringEntity.h"
#include "Arrive.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CPursuit::CPursuit( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance )
	: CSteering			( ::pursuit )
	, m_Target			( NULL )
	, m_EvaderUpdated	( false )
	, m_pArrive			( NULL )
	, m_pEvader			( NULL )
{
	m_pArrive = new CArrive( _Deceleration, _DecelerationBeginsAtDistance );
}


CPursuit::~CPursuit(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect2f CPursuit::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
		if (m_EvaderUpdated)
        {
            m_EvaderUpdated = false;

            Vect2f l_ToEvader = m_Target - _pEntity->GetPosition();

            float l_LookAheadTime = l_ToEvader.Length() / ( _pEntity->GetMaxSpeed() / 2 + m_pEvader->GetSpeed() );

            m_pArrive->SetTarget( m_pEvader->GetPosition() + m_pEvader->GetVelocity() * l_LookAheadTime );

			return m_pArrive->CalculateSteering(_pEntity);
        }
		else
		{
			LOGGER->AddNewLog( ELL_ERROR, "Debe llamar primero al método UpdateEvaderEntity" );
			return NULL;
		}
	}
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo pursuit" );
		return NULL;
    }
}
   
 void CPursuit::UpdateEvaderEntity( CSteeringEntity *_pEvader ) 
{
	m_pEvader		= _pEvader;
	m_EvaderUpdated = true;
}

// -----------------------------------------
//			     PROPERTIES
// -----------------------------------------