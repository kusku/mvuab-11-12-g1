#include "Evade.h"
#include "Arrive.h"
#include "Flee.h"

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

CEvade::CEvade( float _PanicDistance )
	: CSteering			( ::evade )
	, m_pPursuer		( NULL )
	, m_PursuerUpdated	( false )
	, m_PanicDistance	( _PanicDistance )
{
	m_pFlee = new CFlee( _PanicDistance );
}


CEvade::~CEvade(void)
{
	m_pPursuer = NULL;
	CHECKED_DELETE( m_pFlee );
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CEvade::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if ( m_Target != NULL )
	{
		if (m_PursuerUpdated)
		{
			m_PursuerUpdated = false;

			// Vector al objetivo
			Vect3f l_ToPursuer = m_Target - _pEntity->GetPosition();

			// Si estoy a una mayor distancia de la zona de riesgo me salgo
			if ( l_ToPursuer.SquaredLength() > m_PanicDistance * m_PanicDistance) return Vect3f(0,0,0);

			// Calculamos la predicción de la nueva posición del objetivo
			float l_LookAheadTime = l_ToPursuer.Length() / ( _pEntity->GetMaxSpeed() + m_pPursuer->GetSpeed() );

			if ( m_pPursuer != NULL ) 
			{
				m_pFlee->SetTarget( m_pPursuer->GetPosition() + m_pPursuer->GetVelocity() * l_LookAheadTime );
			}
			else
			{
				m_pFlee->SetTarget( m_Target + m_TargetVelocity * l_LookAheadTime );
			}

			return m_pFlee->CalculateSteering(_pEntity);
		}
		else
		{
			LOGGER->AddNewLog( ELL_ERROR, "CEvade::CalculateSteering->Debe llamar primero al método UpdatePursuerEntity" );
			return Vect3f(0.f,0.f,0.f);
		}
	}
	else 
	{
		LOGGER->AddNewLog( ELL_ERROR, "CEvade::CalculateSteering->Debe indicarse el target para el steering de tipo pursuit" );
		return Vect3f(0.f,0.f,0.f);
	}
}

 void CEvade::UpdatePursuerEntity( CSteeringEntity *_pPursuer, float _PanicDistance )
{
    m_pPursuer			= _pPursuer;
    m_PursuerUpdated	= true;
	m_PanicDistance		= _PanicDistance;
}

