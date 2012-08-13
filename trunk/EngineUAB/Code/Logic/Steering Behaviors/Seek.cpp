#include "Seek.h"
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

CSeek::CSeek(void)
	: CSteering		( ::seek )
{
}


CSeek::~CSeek(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CSeek::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
		Vect3f v = m_Target - _pEntity->GetPosition();
		Vect3f l_DesiredVelocity = v.Normalize() * _pEntity->GetMaxSpeed();	

 		// Retorno la direcci�n a dirigirse siempre a m�xima velocidad
        return (l_DesiredVelocity - _pEntity->GetVelocity()); //Aquesta resta s'aplicaria nom�s si volem sumar la part del vector velocitat que genera respecte la velocitat ja existent. 
		//return (l_DesiredVelocity); //Aquesta resta s'aplicaria nom�s si volem sumar la part del vector velocitat que genera respecte la velocitat ja existent. 
    }
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Seek");
		return Vect3f(0,0,0);
    }
}