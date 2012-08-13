#include "Arrive.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

#include "Math\Vector3.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CArrive::CArrive( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance  )
	: CSteering							( ::arrive )
	//, m_Target							( NULL )
	, m_Deceleration					( _Deceleration )
	, m_DecelerationBeginsAtDistance	( _DecelerationBeginsAtDistance )
{
}


CArrive::~CArrive(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CArrive::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
		// Vector desde la posición de la entidad hasta el target
		Vect3f l_ToTarget = m_Target - _pEntity->GetPosition();

		// Distancia al target
		float l_Distance = l_ToTarget.Length();

		Vect3f l_DesiredVelocity;

		// Bajamos la velocidad cuando está dentro del área de "pánico"
		if (l_Distance <= m_DecelerationBeginsAtDistance)
		{
			// Suavizamos la deceleración
			const float l_DecelerationTweaker = 90.3f;

			// Calculamos la velocidad necesaria para alcanzar el target dada la desaceleración
			float l_Speed = l_Distance / ((float)m_Deceleration * l_DecelerationTweaker);

			// La velocidad no puede superar el máximo establecido
			l_Speed = mathUtils::Min(l_Speed, _pEntity->GetMaxSpeed());

			// Utilizamos los valores calculados 
			l_DesiredVelocity = l_ToTarget * l_Speed / l_Distance;
		}
		else 
		{
			// Seek
			l_DesiredVelocity = Vect3f(m_Target - _pEntity->GetPosition()).Normalize() * _pEntity->GetMaxSpeed();
		}

		return (l_DesiredVelocity - _pEntity->GetVelocity());
    }
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Arrive");
		return NULL;
    }
}