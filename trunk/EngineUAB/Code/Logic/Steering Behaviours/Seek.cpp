#include "Seek.h"
#include "Steering Behaviours\SteeringBehavioursDefs.h"
#include "Steering Behaviours\SteeringEntity.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CSeek::CSeek(void)
	: CSteering		( ::seek )
	, m_Target		( )
{
}


CSeek::~CSeek(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect2f CSeek::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
        Vect2f l_DesiredVelocity = Vect2f(m_Target - _pEntity->GetPosition()).Normalize() * _pEntity->GetMaxSpeed();

        return (l_DesiredVelocity - _pEntity->GetVelocity());
    }
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Seek");
		return NULL;
    }
}
   

// -----------------------------------------
//			     PROPERTIES
// -----------------------------------------