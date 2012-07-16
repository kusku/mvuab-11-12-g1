#include "Arrive.h"
#include "Steering Behaviours\SteeringBehavioursDefs.h"
#include "Steering Behaviours\SteeringEntity.h"

#include "Math\Vector2.h"

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

Vect2f CArrive::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
		// Vector desde la posici�n de la entidad hasta el target
		Vect2f l_ToTarget = m_Target - _pEntity->GetPosition();

		// Distancia al target
		float l_Distance = l_ToTarget.Length();

		Vect2f l_DesiredVelocity;

		// Bajamos la velocidad cuando est� dentro del �rea de "p�nico"
		if (l_Distance <= m_DecelerationBeginsAtDistance)
		{
			// Suavizamos la deceleraci�n
			const float l_DecelerationTweaker = 90.3f;

			// Calculamos la velocidad necesaria para alcanzar el target dada la desaceleraci�n
			float l_Speed = l_Distance / ((float)m_Deceleration * l_DecelerationTweaker);

			// La velocidad no puede superar el m�ximo establecido
			l_Speed = mathUtils::Min(l_Speed, _pEntity->GetMaxSpeed());

			// Utilizamos los valores calculados 
			l_DesiredVelocity = l_ToTarget * l_Speed / l_Distance;
		}
		else 
		{
			// Seek
			l_DesiredVelocity = Vect2f(m_Target - _pEntity->GetPosition()).Normalize() * _pEntity->GetMaxSpeed();
		}

		return (l_DesiredVelocity - _pEntity->GetVelocity());
    }
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Arrive");
		return NULL;
    }
}