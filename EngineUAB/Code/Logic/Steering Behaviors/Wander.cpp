#include "Wander.h"
#include "Arrive.h"

#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Scripting\ScriptManager.h"

#include "Utils\Timer.h"
#include "Utils\Random.h"
#include "Math\MathUtils.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#include "Utils\BoostRandomHelper.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CWander::CWander( void )
	: CSteering				( ::wander)
	, m_WanderRadius		( CORE->GetSteeringBehaviourSettingsManager()->GetWanderRadius() )
	, m_WanderDistance		( CORE->GetSteeringBehaviourSettingsManager()->GetWanderDistance())
	, m_WanderJitter		( CORE->GetSteeringBehaviourSettingsManager()->GetWanderRefreshRate())
	, m_Time				( 0.f )
	, m_WanderTarget		( Vect3f ( 0, 0, 0 ) )
	, m_pArrive				( NULL )
	, m_WanderOrientation	( 0.f )
{
	// Inicializo el comportamiento de llegada al objetivo para frenarlo
	m_pArrive = new CArrive( CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelaration(), CORE->GetSteeringBehaviourSettingsManager()->GetArriveDecelarationDistance() );
}

CWander::CWander( float _RefreshRate, float _DistanciaCirculo, float _RadioCirculo, eDeceleration _Deceleration, float _DecelerationBeginsAtDistance )
	: CSteering				( ::wander)
	, m_WanderRadius		( _RadioCirculo )
	, m_WanderDistance		( _DistanciaCirculo )
	, m_WanderJitter		( _RefreshRate )
	, m_Time				( 0.f )
	, m_WanderTarget		( Vect3f ( 0, 0, 0 ) )
	, m_pArrive				( NULL )
	, m_WanderOrientation	( 0.f )
{
	// Inicializo el comportamiento de llegada al objetivo para frenarlo
	m_pArrive = new CArrive( _Deceleration, _DecelerationBeginsAtDistance );
}


CWander::~CWander(void)
{
	CHECKED_DELETE( m_pArrive );
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CWander::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if ( m_Target != NULL )
	{
		m_Time += CORE->GetTimer()->GetElapsedTime();  // TotalMilliseconds;
	
		m_pArrive->SetTarget(m_Target);

		// Si és el tiempo = 0 o bien superamos el nº de refrescos por segundo (nervio que le damos) o estamos a una distancia del objetivo dentro de los mínimos
		/*if ( m_Time == 0.0f || m_Time >= m_WanderJitter || ( ( m_Target.Distance( _pEntity->GetPosition() ) ) <= m_WanderMinimumDistance ) )
        {*/
			//m_Time = 0.1f;
	
			//CRandom	l_Randomize;
			
			// determine a random vector between _vect1 and _vect2
			//float l_AngleTmp = mathUtils::Deg2Rad( l_Randomize.getRandFloat( -360, 361 ) );
			float l_JitterThisTimeSlice = m_WanderJitter * m_Time;
			
			//float l_TargetOrientation = BoostRandomHelper::GetFloat(0, static_cast<float>(mathUtils::PiTimes(2))) * l_JitterThisTimeSlice ;

			//Vect3f l_TargetOrientationVector;
			//l_TargetOrientationVector.GetXZFromAngle(l_TargetOrientation);
			//l_TargetOrientationVector.Normalize();

			//// Buscamos la posició aleatoria en el círculo
			///*float l_PosicionXCirculo = _pEntity->GetPosition().x + m_WanderDistance + ( l_TargetOrientation * m_WanderRadius);
			//float l_PosicionZCirculo = _pEntity->GetPosition().z + m_WanderDistance + ( ( float )mathUtils::Cos(l_AngleTmp) * m_WanderRadius);*/
			//Vect3f l_Target = _pEntity->GetPosition() + m_WanderDistance + ( l_TargetOrientationVector * m_WanderRadius);

			//float l_NewAngle = static_cast<float> (BoostRandomHelper::GetFloat(-1, 1));
			//float l_NewAngleGraus = mathUtils::Rad2Deg(l_NewAngle);
			//m_WanderOrientation += l_NewAngle * l_JitterThisTimeSlice;
			
			// Pillamos un angulo cualquiera
			float l_NewAngle = static_cast<float> (BoostRandomHelper::GetFloat(-360, 360));
			float l_NewAngleRadians = mathUtils::Deg2Rad(l_NewAngle);

			// Le añadimos este al anterior 
			m_WanderOrientation += l_NewAngleRadians;


			float l_TargetOrientation = m_WanderOrientation + _pEntity->GetYaw();
			Vect3f l_TargetOrientationVector;
			l_TargetOrientationVector.GetXZFromAngle(l_TargetOrientation);
			l_TargetOrientationVector.Normalize();

			Vect3f l_Front = _pEntity->GetFront();
			l_Front.Normalize();
			Vect3f l_Target = _pEntity->GetPosition() + m_WanderDistance * l_Front CORE->GetTimer()->GetElapsedTime();;

			l_Target += m_WanderRadius * l_TargetOrientationVector;
				 
			// Obtengo un vector dirección random hacia la posición del target
			//m_WanderTarget +=  Vect3f ( BoostRandomHelper::GetFloat(-1, 1) * l_JitterThisTimeSlice, 0.f, BoostRandomHelper::GetFloat(-1, 1) * l_JitterThisTimeSlice );

			//// Vector unitario
			//m_WanderTarget.Normalize();

			//// Incrementa la longitud del vector según el radio del wander circle
			//m_WanderTarget *= m_WanderRadius;

			//// Mueve el objetivo hacia una distancia de la posició de la entidad en frente de esta
			//Vect3f l_Target = _pEntity->GetPosition() +  m_WanderTarget + Vect2f( m_WanderDistance, 0 );
			//

			m_pArrive->SetTarget( l_Target );
			SetTarget (m_pArrive->GetTarget());
		//}
		
		return m_pArrive->CalculateSteering(_pEntity);
	}
	else 
	{
		LOGGER->AddNewLog( ELL_ERROR, "CWander::CalculateSteering->Debe indicarse el target para el steering de tipo wander" );
		return Vect3f(0.f,0.f,0.f);
	}
}