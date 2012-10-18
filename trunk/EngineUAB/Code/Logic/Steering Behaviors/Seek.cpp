#include "Seek.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Utils\Timer.h"

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
	if (!m_Target.IsZero())
    {
		// Si està dins la distancia d'atac
		/*if ( _pEntity->GetPosition().SqDistance( GetTarget() ) < ( 2.f * 2.f ))
		{
			_pEntity->SetVelocity( Vect3f(0,0,0) );
			return Vect3f(0,0,0);
		}*/

		Vect3f v = m_Target - _pEntity->GetPosition();
		Vect3f l_DesiredVelocity = v.Normalize() * _pEntity->GetMaxSpeed() * CORE->GetTimer()->GetElapsedTime();	

 		// Retorno la dirección a dirigirse siempre a máxima velocidad
        return (l_DesiredVelocity - _pEntity->GetVelocity()); //Aquesta resta s'aplicaria només si volem sumar la part del vector velocitat que genera respecte la velocitat ja existent. 
		//return (l_DesiredVelocity); //Aquesta resta s'aplicaria només si volem sumar la part del vector velocitat que genera respecte la velocitat ja existent. 
    }
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Seek");
		return Vect3f(0,0,0);
    }
}