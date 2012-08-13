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

CFlee::CFlee( float _PanicDistance )
	: CSteering			( ::flee )
	, m_PanicDistance	( _PanicDistance )
{
}


CFlee::~CFlee(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CFlee::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
	{
		if ( (_pEntity->GetPosition()).SqDistance(m_Target) > m_PanicDistance )
			return Vect3f ( 0.0f, 0.0f, 0.0f );

		Vect3f l_DesiredVelocity = Vect3f(_pEntity->GetPosition() - m_Target).Normalize() * _pEntity->GetMaxSpeed();

		return (l_DesiredVelocity - _pEntity->GetVelocity());
	}
	else 
	{
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo flee");
		return NULL;
	}
}

// -----------------------------------------
//			     PROPERTIES
// -----------------------------------------