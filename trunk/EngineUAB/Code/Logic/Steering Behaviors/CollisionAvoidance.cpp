#include <string>
#include <map>

#include "CollisionAvoidance.h"
#include "StatesMachine\BaseGameEntity.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\Evade.h"

//#include "Characters\CharacterManager.h"
//#include "Characters\Character.h"
#include "Utils\TemplatedVectorMapManager.h"
//#include "GameProcess.h"

#include "PhysicsManager.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"

#include "RenderManager.h"
#include "Fonts\FontManager.h"

#include "Utils\Timer.h"
#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CCollisionAvoidance::CCollisionAvoidance( float _PanicDistance )
	: CSteering					( ::collision_avoidance )
	, m_DetectionRayLength		( CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerLength() )
	, m_NearestObstacle			( FLT_MAX )
	, m_NearestSteering			( Vect3f(0.f,0.f,0.f) )
	, m_pEntity					( NULL )
	, m_pEvade					( NULL )
	, m_PanicDistance			( _PanicDistance )
	, m_pPursuer				( NULL )
{
	m_pEvade	= new CEvade( _PanicDistance );
	m_pPursuer	= new CSteeringEntity();
}

CCollisionAvoidance::CCollisionAvoidance( float _PanicDistance, float _MinDetectionLength )
	: CSteering					( ::collision_avoidance )
	, m_DetectionRayLength		( _MinDetectionLength )
	, m_NearestObstacle			( FLT_MAX )
	, m_NearestSteering			( Vect3f(0.f,0.f,0.f) )
	, m_pEntity					( NULL )
	, m_pEvade					( NULL )
	, m_PanicDistance			( _PanicDistance )
	, m_pPursuer				( NULL )
{
	m_pEvade	= new CEvade( _PanicDistance );
	m_pPursuer	= new CSteeringEntity();
}


CCollisionAvoidance::~CCollisionAvoidance(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CCollisionAvoidance::GetObstaclePosition( CPhysicUserData * _pUserData ) 
{
	Vect3f l_ObstaclePosition;
	l_ObstaclePosition = _pUserData->GetSteeringEntity()->GetPosition();

	return l_ObstaclePosition;
}

Vect3f CCollisionAvoidance::GetObstacleVelocity( CPhysicUserData *_pUserData ) 
{
	Vect3f l_ObstacleVelocity;
	l_ObstacleVelocity = _pUserData->GetSteeringEntity()->GetVelocity();

	return l_ObstacleVelocity;
}

Vect3f CCollisionAvoidance::CalculateSteering( CSteeringEntity *_pEntity )	
{
	m_NearestObstacle = FLT_MAX;
	m_NearestSteering.SetZero();
	
	// Si no tengo velocidad retorno
	if ( _pEntity->GetVelocity().SquaredLength() <= 0 )
	{
		return m_NearestSteering;
	}

	// Almacenamos la entidad tratada
	m_pEntity = _pEntity;

	// Creo un vector de linias que seran los rayos a lanzar
	CreateFeelers(CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerAngle());

	// Distancia mínima de colisión
	float l_MinDistanceToCollision = m_DetectionRayLength * _pEntity->GetSpeed() * CORE->GetTimer()->GetElapsedTime();;
	
	// La caja de detección es proporcional a la velocidad del agente
	float l_DetectionBoxLength = m_DetectionRayLength + ( _pEntity->GetSpeed() / _pEntity->GetMaxSpeed() ) * m_DetectionRayLength * CORE->GetTimer()->GetElapsedTime();;
	
	// Guardamos aquí el ID de la entidad más cercana
	//CBaseGameEntity* l_NearestObstacle	= NULL;
 	
	m_pEntity->GetController()->SetGroup(ECG_RAY_IA_GRAPH);

	// Analizamos sólo los obstáculos dentro del alcance
	SCollisionInfo l_Info; 
	for ( size_t flr = 0; flr < m_Feelers.size(); ++flr)
	{
		CPhysicUserData * l_pUserData = ShootCollisionRay( _pEntity, m_Feelers[flr], l_Info, l_DetectionBoxLength );

		// Sabemos que no és ni el player ni él mismo y debemos calcular otra posición
		if ( l_pUserData != NULL ) 
		{
			//calculate the distance between the positions of the entities	
		
			Vect3f l_ObstaclePosition; 
		
			// Si no devuelve nada no retorno fuerza
			l_ObstaclePosition = GetObstaclePosition(l_pUserData);
			if ( l_ObstaclePosition == NULL )
				return m_NearestSteering;

			// Obtengo el radio del obstaculo
			float  l_Radius = l_pUserData->GetRadius();

				//l_ToEntity = _pEntity->GetPosition() - l_ObstaclePosition;

		// distancia mínima al obstáculo para iniciar la separación del obstáculo
			float l_MinDistanceToCenter = l_MinDistanceToCollision + l_Radius;
		
			// distancia de contacto
			float l_TotalRadius = _pEntity->GetBoundingRadius() + l_pUserData->GetSteeringEntity()->GetBoundingRadius();
		
			// centro del obstáculo relativo a la posición de la entidad
			Vect3f l_RelativePosition = l_ObstaclePosition - _pEntity->GetPosition();

										// Miro el angulo entre los dos vectores, donde mira la entidad y el vector dirección hacia el objetivo
		/*float l_Angle = _pEntity->GetYaw();
		float l_Angle1 = _pEntity->GetAngle();
		float l_Angle2 = _pEntity->GetAngle2();

		float l_Angle3 = _pEntity->GetPosition().AngleWithVector( Vect3f(1,0,0) );*/

		//float l_Angle4 = _pEntity->GetPosition().xzToAngle();
		//Vect3f l_NewPos = _pEntity->GetPosition().GetXZFromAngle(mathUtils::Deg2Rad(l_Angle4));

		//Vect3f l_Front = _pEntity->GetPosition().GetAngleY();www
			float l_ForwardComponent = l_RelativePosition.Dot( _pEntity->GetFront() );
			float l_fAngle			 = mathUtils::ACos<float>( l_ForwardComponent );wddd
			Vect3f l_ForwardOffset	 = l_ForwardComponent * _pEntity->GetFront();

			// Offset del forward al centro del obstáculo
			Vect3f l_OffForwardOffset = l_RelativePosition - l_ForwardOffset;
			//Vect3f l_OffForwardOffset = l_Info.m_fDistance;

			// test para ver si el obstáculo es "pisado" por el agente
			bool inCylinder = l_OffForwardOffset.Length() < l_TotalRadius;			// Miramos si la distancia entre ellos es más pequeña que sus radios
			bool nearby		= l_ForwardComponent < l_MinDistanceToCenter;			// Miramos si el Cos(Angulo) és menor que la distancia que queremos a partir de la que esquivamos
			bool inFront	= l_ForwardComponent > 0;								// Miramos si está dentro de los 90º

			// si se cumple una de las condiciones, alejamos del centro del obstáculo
			if ( inCylinder || inFront || nearby ) 
			{
				float l_Length = (l_OffForwardOffset * -1).Length();

				if (l_Length < m_NearestObstacle)
				{
					m_NearestObstacle = l_Length;
					m_NearestSteering = l_OffForwardOffset * -1;
					//this.obstacle = obstacle;
				}
			}
			/*LOGGER->AddNewLog( ELL_INFORMATION, " Nom de la colisicó  %s", l_pUserData->GetName().c_str() );
			LOGGER->AddNewLog( ELL_INFORMATION, " Nom de l'entitat  %s", _pEntity->GetName().c_str() );*/

			//this->EnforceNonPenetrationConstraint( _pEntity, l_pUserData );
		}
	}

	m_pEntity->GetController()->SetGroup(ECG_ENEMY);

	return m_NearestSteering * CORE->GetTimer()->GetElapsedTime();
}

Vect3f CCollisionAvoidance::CalculateSteering2( CSteeringEntity *_pEntity )	
{
	// Almacenamos la entidad tratada
	m_pEntity = _pEntity;

	// Creo los bigotitos...
	CreateFeelers(CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerAngle());

	// Almaceno info de colision
	SCollisionInfo sInfo;
	
	// La fuerza resultante a devolver
	Vect3f l_SteeringForce;
	l_SteeringForce.SetZero();

	// Registra la media de vector dirección de los obstaculos
	Vect3f l_AverageVelocity;
	l_AverageVelocity.SetZero();

	// Registra la media de vector posición de los obstaculos
	Vect3f l_AveragePosition;
	l_AveragePosition.SetZero();

	// Registra el nº de objetos que se col·lisiona
	int	l_ObstaclesCount = 0;

	// Distancia al punto de intersección más proximo
	float l_DistanceToClosestIP = 0.f;
	
	// Punto más proximo de colision
	Vect3f l_ClosestPoint;
	l_ClosestPoint.SetZero();

	// Elementos ya tratados
	std::map<std::string, std::string> l_ObstaclesTreated;
	l_ObstaclesTreated.clear();

	// Vector normal de la superficie mas proxima
	Vect3f l_ClosestNormal;
	l_ClosestNormal.SetZero();

	for ( size_t flr = 0; flr < m_Feelers.size(); ++flr)
	{
		// Siempre hay que pasar la dirección normalizada
		CPhysicUserData * l_Data = ShootCollisionRay( _pEntity, m_Feelers[flr], sInfo, m_DetectionRayLength );

		// Si ha colisionado con algo miramos si está más proximo
		if ( l_Data ) 
		{
			if ( l_Data->GetName().compare(_pEntity->GetName() ) != 0 ) 
			{
				/*std::string l_Name = l_Data->GetName();
				if ( l_ObstaclesTreated.find(l_Name) == l_ObstaclesTreated.end() )
				{
					l_ObstaclesTreated[l_Name] = l_Name;
					++l_ObstaclesCount;
				}
				else
					continue;*/
				++l_ObstaclesCount;
			}
			else 
				continue;

			// Ahora pillamos la posición y velocidad del obstaculo
			l_AveragePosition += GetObstaclePosition(l_Data);
			l_AverageVelocity += GetObstacleVelocity(l_Data);
		}
	}

	if ( l_ObstaclesCount > 0 ) 
	{
		l_AveragePosition = l_AveragePosition / (float)l_ObstaclesCount - _pEntity->GetPosition();
		l_AverageVelocity = l_AverageVelocity / (float)l_ObstaclesCount - _pEntity->GetVelocity();
	
		m_pPursuer->SetVelocity(l_AverageVelocity);
		m_pPursuer->SetPosition(l_AveragePosition);

		m_pEvade->SetTarget(l_AveragePosition);
		m_pEvade->UpdatePursuerEntity( m_pPursuer, m_PanicDistance );
		l_SteeringForce = m_pEvade->CalculateSteering(_pEntity);
	}

	return l_SteeringForce;
}

Vect3f CCollisionAvoidance::CalculateSteering3( CSteeringEntity *_pEntity )	
{
	// Almacenamos la entidad tratada
	m_pEntity = _pEntity;

	// Creo los bigotitos...
	CreateFeelers(CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerAngle());

	// Almaceno info de colision
	SCollisionInfo sInfo;
	
	// La fuerza resultante a devolver
	Vect3f l_SteeringForce;
	l_SteeringForce.SetZero();

	// Registra la velocidad entre el target y el caracter
	Vect3f l_RelativeVelocity;
	l_RelativeVelocity.SetZero();

	// Registra la posición entre el target y el caracter
	Vect3f l_RelativePosition;
	l_RelativePosition.SetZero();

	// Guarda el primer tiempo de colisión
	float l_ShortestTime = FLT_MAX;

	// Info para guardar el target que colisiona y otra info necesaria para evitar recalculos
	CSteeringEntity* l_FirstTarget		= NULL;
	float l_FirstMinSeparation			= FLT_MAX;
	float l_FirstDistance				= FLT_MAX;
	float l_FirstMinCollisionDistance	= FLT_MAX;
	Vect3f l_FirstRelativeVelocity;
	l_FirstRelativeVelocity.SetZero();
	Vect3f l_FirstRelativePosition;
	l_FirstRelativePosition.SetZero();
	float l_Distance 				= 0.f;

	//	// Elementos ya tratados
	int l_ObstaclesCount = 0;
	std::map<std::string, std::string> l_ObstaclesTreated;
	l_ObstaclesTreated.clear();
			
	// Radio del obstaculo
	float  l_Radius		= 0.f;

	// distancia mínima al obstáculo para iniciar la separación del obstáculo
	//float l_MinDistanceToCenter = l_MinDistanceToCollision + l_Radius;
	
	// distancia de contacto
	float l_TotalRadius = 0.f;

	// Distancia mínima de colisión
	float l_MinDistanceToCollision = 5.150f * _pEntity->GetSpeed();

	// Desactivo la colisión de la entidad
	_pEntity->GetController()->SetCollision(false);
	_pEntity->GetController()->SetGroup( ECG_RAY_SHOOT);
	
	for ( size_t flr = 0; flr < m_Feelers.size(); ++flr)
	{
		// Siempre hay que pasar la dirección normalizada
		CPhysicUserData * l_pUserData = ShootCollisionRay( _pEntity, m_Feelers[flr], sInfo, m_DetectionRayLength );

		// Si ha colisionado con algo miramos si está más proximo
		if ( l_pUserData ) 
		{
			if ( l_pUserData->GetName().compare(_pEntity->GetName() ) != 0 ) 
			{
				std::string l_Name = l_pUserData->GetName();
				if ( l_ObstaclesTreated.find(l_Name) == l_ObstaclesTreated.end() )
				{
					l_ObstaclesTreated[l_Name] = l_Name;
					++l_ObstaclesCount;
				}
				else
					continue;
			}
			else 
				continue;

			// Ahora pillamos la posición y velocidad del obstaculo
			l_RelativePosition		= GetObstaclePosition(l_pUserData) - _pEntity->GetPosition();
			l_RelativeVelocity		= GetObstacleVelocity(l_pUserData) - _pEntity->GetVelocity();
			float l_RelativeSpeed	= l_RelativeVelocity.Length();

			// Calculamos el tiempo en el que colisionaran
			float l_fAngle	= l_RelativePosition.Dot( l_RelativeVelocity );			// Obtenemos el coseno del angulo
			l_fAngle		= mathUtils::ACos<float>( l_fAngle );					// Obtenemos el angulo
			float l_fAngleDegress = mathUtils::Rad2Deg(l_fAngle);

			float l_TimeToCollision = (l_fAngle) / (l_RelativeSpeed * l_RelativeSpeed)/100;

			// distancia mínima al obstáculo para iniciar la separación del obstáculo
			float l_MinDistanceToCenter = l_MinDistanceToCollision + _pEntity->GetBoundingRadius();

			// Obtengo el radio del obstaculo
			l_Radius				= l_pUserData->GetRadius();
			l_TotalRadius			= l_Radius + l_MinDistanceToCenter;

			// Comprovamos si va a colisionar al final
			l_Distance = l_RelativePosition.Length();		// Distancia al objetivo
			float l_MinSeparation = l_Distance - l_RelativeSpeed * l_TimeToCollision;

			LOGGER->AddNewLog(ELL_INFORMATION,"minima separación : %d", l_MinSeparation);
			LOGGER->AddNewLog(ELL_INFORMATION,"Time colision: %d", l_TimeToCollision);
			if ( l_MinSeparation > l_TotalRadius )
				continue;

			// Comprovamos si és el más corto
			if ( ( l_TimeToCollision > 0) && ( l_TimeToCollision < l_ShortestTime ) )
			{
				// Almacenamos el tiempo, target i datos
				l_ShortestTime				= l_TimeToCollision;
				l_FirstTarget				= l_pUserData->GetSteeringEntity();
				l_FirstMinSeparation		= l_MinSeparation;
				l_FirstDistance				= l_Distance;
				l_FirstRelativePosition		= l_RelativePosition;
				l_FirstRelativeVelocity		= l_RelativeVelocity;
				l_FirstMinCollisionDistance = l_TotalRadius;
			}
		}
	}

	if ( l_FirstTarget ) 
	{
		// Ya debería tener el elemento más cercano encontrado y calculamos la fuerza steering basada en la actual posición
		if ( ( l_FirstMinSeparation <= 0 ) || ( l_FirstDistance < l_FirstMinCollisionDistance ) )
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "Opció A" );
			l_RelativePosition = l_FirstTarget->GetPosition() - _pEntity->GetPosition();
		}
		// En otro caso calculamos la futura posición relativa
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "Opció BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB" );
			l_RelativePosition = l_FirstRelativePosition + l_FirstRelativeVelocity * l_ShortestTime;
		}

		// Evitamos el target
		l_RelativePosition.Normalize();
		l_SteeringForce = l_RelativePosition * _pEntity->GetMaxAcceleration();
		
		/*m_pPursuer->SetPosition (l_RelativePosition);
		m_pEvade->SetTarget( l_RelativePosition );
		m_pEvade->UpdatePursuerEntity( m_pPursuer, 4.f );
		l_SteeringForce = m_pEvade->CalculateSteering(m_pPursuer);*/
	}

	// Vuelvo a activar la colisión de la entidad
	_pEntity->GetController()->SetCollision(true);
	_pEntity->GetController()->SetGroup( ECG_ENEMY);

	
	return l_SteeringForce;
}

Vect3f CCollisionAvoidance::CalculateSteering1( CSteeringEntity *_pEntity )	
{
	// Almacenamos la entidad tratada
	m_pEntity = _pEntity;

	// Creo los bigotitos...
	CreateFeelers(CORE->GetSteeringBehaviourSettingsManager()->GetCollisionDetectionFeelerAngle());

	// Almaceno info de colision
	SCollisionInfo sInfo;
	
	// La fuerza resultante a devolver
	Vect3f l_SteeringForce;
	l_SteeringForce.SetZero();

	// Registra la velocidad entre el target y el caracter
	Vect3f l_RelativeVelocity;
	l_RelativeVelocity.SetZero();

	// Registra la posición entre el target y el caracter
	Vect3f l_RelativePosition;
	l_RelativePosition.SetZero();

	// Guarda el primer tiempo de colisión
	float l_ShortestTime = FLT_MAX;

	// Info para guardar el target que colisiona y otra info necesaria para evitar recalculos
	CSteeringEntity* l_FirstTarget		= NULL;
	float l_FirstMinSeparation			= FLT_MAX;
	float l_FirstDistance				= FLT_MAX;
	float l_FirstMinCollisionDistance	= FLT_MAX;
	Vect3f l_FirstRelativeVelocity;
	l_FirstRelativeVelocity.SetZero();
	Vect3f l_FirstRelativePosition;
	l_FirstRelativePosition.SetZero();
	float l_Distance 				= 0.f;

	//	// Elementos ya tratados
	int l_ObstaclesCount = 0;
	std::map<std::string, std::string> l_ObstaclesTreated;
	l_ObstaclesTreated.clear();
			
	// Radio del obstaculo
	float  l_Radius		= 0.f;

	// distancia mínima al obstáculo para iniciar la separación del obstáculo
	//float l_MinDistanceToCenter = l_MinDistanceToCollision + l_Radius;
	
	// distancia de contacto
	float l_TotalRadius = 0.f;

	// Distancia mínima de colisión
	float l_MinDistanceToCollision = 5.150f * _pEntity->GetSpeed();

	// Desactivo la colisión de la entidad
	_pEntity->GetController()->SetCollision(false);
	_pEntity->GetController()->SetGroup( ECG_RAY_SHOOT);
	
	// Por cada rayo miramos con qué colisiona y buscamos el punto más cercano de colision de todos los caracteres. Puede ser que varios rayos colisionen
	// con el mismo objeto pero siempre obtendremos el punto más cercano de colisión
	for ( size_t flr = 0; flr < m_Feelers.size(); ++flr)
	{
		// Siempre hay que pasar la dirección normalizada
		CPhysicUserData * l_pUserData = ShootCollisionRay( _pEntity, m_Feelers[flr], sInfo, m_DetectionRayLength );

		// Si ha colisionado con algo miramos si está más proximo
		if ( l_pUserData ) 
		{
			if ( l_pUserData->GetName().compare(_pEntity->GetName() ) != 0 ) 
			{
				std::string l_Name = l_pUserData->GetName();
				if ( l_ObstaclesTreated.find(l_Name) == l_ObstaclesTreated.end() )
				{
					l_ObstaclesTreated[l_Name] = l_Name;
					++l_ObstaclesCount;
				}
				else
					continue;
			}
			else 
				continue;

			// Ahora pillamos la posición y velocidad del obstaculo
			l_RelativePosition		= GetObstaclePosition(l_pUserData) - _pEntity->GetPosition();
			l_RelativeVelocity		= GetObstacleVelocity(l_pUserData) - _pEntity->GetVelocity();
			float l_RelativeSpeed	= l_RelativeVelocity.Length();

			// Calculamos el tiempo en el que colisionaran
			float l_fAngle	= l_RelativePosition.Dot( l_RelativeVelocity );			// Obtenemos el coseno del angulo
			l_fAngle		= mathUtils::ACos<float>( l_fAngle );					// Obtenemos el angulo
			float l_fAngleDegress = mathUtils::Rad2Deg(l_fAngle);

			// distancia mínima al obstáculo para iniciar la separación del obstáculo
			float l_MinDistanceToCenter = l_MinDistanceToCollision + _pEntity->GetBoundingRadius();

			// Obtengo el radio del obstaculo
			l_Radius				= l_pUserData->GetRadius();
			l_TotalRadius			= l_Radius + l_MinDistanceToCenter;

			// Comprovamos si va a colisionar al final
			l_Distance = l_RelativePosition.Length();		// Distancia al objetivo
			//float l_MinSeparation = l_Distance - l_RelativeSpeed * l_TimeToCollision;
			if ( l_Distance > l_FirstDistance )
			{
				continue;
			}

			if ( l_Distance <= l_FirstDistance ) 
			{
				// Almacenamos el tiempo, target i datos
				l_FirstTarget				= l_pUserData->GetSteeringEntity();
				//l_FirstMinSeparation		= l_MinSeparation;
				l_FirstDistance				= l_Distance;
				l_FirstRelativePosition		= l_RelativePosition;
				l_FirstRelativeVelocity		= l_RelativeVelocity;
				l_FirstMinCollisionDistance = l_TotalRadius;
			}
		}
	}

	if ( l_FirstTarget ) 
	{
		//// Ya debería tener el elemento más cercano encontrado y calculamos la fuerza steering basada en la actual posición
		
		Vect3f l_Forward = m_pEntity->GetHeading();
		float l_fAngle	= l_FirstRelativePosition.Dot( l_Forward );			// Obtenemos el coseno del angulo

		// Solo aseguro que el vehículo está en frente pero ya lo ser...
		if ( l_fAngle <= 0 )
			return l_SteeringForce;

		// Rayo + distancia
		Vect3f l_Ray = l_Forward * m_DetectionRayLength;
		Vect3f l_Projection = l_Forward * l_fAngle;		// Heading hacia el objeto que colisiono
		//l_FirstRelativePosition
		//l_FirstDistance
		
		Vect3f l_DesiredVelocity;

		if ( l_FirstDistance <= l_FirstMinCollisionDistance && l_Projection.SquaredLength() < l_Ray.SquaredLength() )
		{
			// cojemos la máxima velocidad en la dirección que iba
			l_DesiredVelocity = l_Forward * _pEntity->GetMaxSpeed();	
			
			// Rotamos
			l_DesiredVelocity.RotateY(90);
			
			// Escalamos
			//l_DesiredVelocity *= (1 - l_Projection.Length() / l_Ray.Length());
			
			l_SteeringForce = l_DesiredVelocity - m_pEntity->GetVelocity();
		}


		//			var dist:Number = projection.cloneVector().subtract(diff).length; //get the distance between the circle and vehicle
		//			if(dist < circles[i].radius + width && projection.length < ray.length) 
//			{
//				//if the circle is in your path (radius+width to check the full size of the vehicle)
//				//projection.length and ray.length make sure you are within the max distance
//				var force:Vector2D = forward.cloneVector().multiply(maxSpeed); //get the max force
//				force.angle += diff.sign(velocity) * Math.PI / 2; //rotate it away from the cirlce
//				//PI / 2 is 90 degrees, vector's angles are in radians
//				//sign returns whether the vector is to the right or left of the other vector
//				force.multiply(1 - projection.length / ray.length); //scale the force so that a far off object
//				//doesn't drastically change the velocity
//				velocity.add(force);//change the velocity
//				velocity.multiply(projection.length / ray.length);//and scale again
//			}

		/*m_pPursuer->SetPosition (l_FirstRelativePosition);
		m_pPursuer->*/
		/*m_pEvade->SetTarget( l_FirstRelativePosition );
		m_pEvade->UpdatePursuerEntity( m_pPursuer, 31.f );
		l_SteeringForce = m_pEvade->CalculateSteering(m_pPursuer);*/
	}

	// Vuelvo a activar la colisión de la entidad
	_pEntity->GetController()->SetCollision(true);
	_pEntity->GetController()->SetGroup( ECG_ENEMY);

	
	return l_SteeringForce;
}

///private var checkLength:Number = 100;//the distance to look ahead for circles

//public function avoidObstacles(circles:Array):void 
//{
//	for(var i:int = 0; i < circles.length; i++) 
//	{//loop through the array of obstacles
//		var forward:Vector2D = velocity.cloneVector().normalize();//get the forward vector
//		var diff:Vector2D = circles[i].position.cloneVector().subtract(position);//get the difference between the circle and the vehicle
//		var dotProd:Number = diff.dotProduct(forward);//get the dot product
//		//this will be used for projection
//		//much like in the <a href="http://rocketmandevelopment.com/2010/05/19/separation-of-axis-theorem-for-collision-detection/">SAT</a>
//
//		if(dotProd > 0) 
//		{ //if this object is in front of the vehicle
//
//			var ray:Vector2D = forward.cloneVector().multiply(checkLength); //get the ray
//			var projection:Vector2D = forward.cloneVector().multiply(dotProd); //project the forward vector
//			var dist:Number = projection.cloneVector().subtract(diff).length; //get the distance between the circle and vehicle
//
//			if(dist < circles[i].radius + width && projection.length < ray.length) 
//			{
//				//if the circle is in your path (radius+width to check the full size of the vehicle)
//				//projection.length and ray.length make sure you are within the max distance
//				var force:Vector2D = forward.cloneVector().multiply(maxSpeed); //get the max force
//				force.angle += diff.sign(velocity) * Math.PI / 2; //rotate it away from the cirlce
//				//PI / 2 is 90 degrees, vector's angles are in radians
//				//sign returns whether the vector is to the right or left of the other vector
//				force.multiply(1 - projection.length / ray.length); //scale the force so that a far off object
//				//doesn't drastically change the velocity
//				velocity.add(force);//change the velocity
//				velocity.multiply(projection.length / ray.length);//and scale again
//			}
//		}
//	}
//}



 void CCollisionAvoidance::EnforceNonPenetrationConstraint( CSteeringEntity *_pEntity, CPhysicUserData *_pUserDataObstacle )
{
	//calculate the distance between the positions of the entities
	CPhysicActor * l_Actor = _pUserDataObstacle->GetActor();
	CPhysicController * l_Controller = _pUserDataObstacle->GetController();

	Vect3f l_ToEntity;

	if ( l_Actor )
		l_ToEntity = _pEntity->GetPosition() - l_Actor->GetPosition();
	else if ( l_Controller )
		l_ToEntity = _pEntity->GetPosition() - l_Controller->GetPosition();
	else
		return;

    float l_DistFromEachOther = l_ToEntity.Length();

    // if this distance is smaller than the sum of their radius then this entity must be moved away in the direction parallel to the ToEntity vector   
	float l_AmountOfOverLap =  _pEntity->GetBoundingRadius() + _pEntity->GetBoundingRadius()/2 + _pUserDataObstacle->GetRadius() - l_DistFromEachOther;
	
    if (l_AmountOfOverLap >= 0)
    {
        // move the entity a distance away equivalent to the amount of overlap.
		Vect3f l_NewPos = _pEntity->GetPosition() + (l_ToEntity / l_DistFromEachOther) * l_AmountOfOverLap;
        _pEntity->SetPosition( l_NewPos );
    }
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void CCollisionAvoidance::CreateFeelers( float _Angle )
{
	m_Feelers.clear();

	/*for ( float i = -30.f; i < _Angle; i += 2.f ) 
	{

		Vect3f l_Front = m_pEntity->GetFront();
		l_Front.y = 0;
		l_Front.RotateY(mathUtils::Deg2Rad(i));
		m_Feelers.push_back(l_Front);
	}*/
		

	float l_Increment = CORE->GetSteeringBehaviourSettingsManager()->GetDetectionFeelerAngleIncrement();
	for ( float i = -_Angle/2; i < _Angle/2; i += l_Increment ) 
	{
		m_Feelers.push_back(GetDirectionRay(i));
	}

}

CPhysicUserData* CCollisionAvoidance::ShootCollisionRay( CSteeringEntity *_pEntity, const Vect3f &_Direction, SCollisionInfo& _Info, float _MaxDistance )
{
	//SCollisionInfo sInfo;
	/*uint32 l_Mask = 0;
	l_Mask = 1 << 0;*/
	
	uint32 l_Mask; // = 1 << ECG_PLAYER;
	l_Mask |= 1 << ECG_ENEMY;
	l_Mask |= 1 << ECG_DYNAMIC_OBJECTS;
	//l_Mask |= 1 << ECG_ESCENE;

	//Vect3f l_InitialPosition = _pEntity->GetInitialPositionToThrowRay();
	Vect3f l_InitialPosition = _pEntity->GetPosition();
	l_InitialPosition.y += _pEntity->GetHeight()/2;

	Vect3f l_FinalPosition   = _Direction;
	l_FinalPosition.Normalize();
	// Siempre hay que pasar la dirección normalizada
	CPhysicUserData * l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_InitialPosition, l_FinalPosition, l_Mask, _Info, _MaxDistance );
	
	//Vect3f l_Direction		 = _pEntity->GetFront();
	//l_Direction.Normalize();
	//CPhysicUserData * l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_InitialPosition, l_Direction, l_Mask , _Info, _MaxDistance );
	
	// El rayo frontal no funcionó o no col·lisionó. Lanzo otro en un lado del fustrum
	//if ( !l_Data ) 
	//{
	//	Vect3f l_Front = _pEntity->GetFront();
	//	l_Front.RotateY(mathUtils::Deg2Rad(30.f));
	//	l_Dir = Vect3f ( GetTarget().x + l_Front.x + l_Front.x  , GetTarget().y + _pEntity->GetHeight(), GetTarget().z + l_Front.z + l_Front.z );
	//	l_Dir.Normalize();
	//	l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_ObstaclePosition, l_Dir, l_Mask , s, _MaxDistance );

	//	if ( l_Data )
	//	{
	//		std::string l_Name = l_Data->GetName().c_str();
	//		if ( l_Name.compare(_pEntity->GetName()) != 0 ) 
	//		{
	//			LOGGER->AddNewLog( ELL_INFORMATION, "Colisió LATERAL 30º d'enemic %s amb el player: %s", _pEntity->GetName().c_str(), l_Data->GetName().c_str());
	//		}
	//	}
	//}

	//// El rayo frontal no funcionó o no col·lisionó. Lanzo otro en un lado del fustrum
	//if ( !l_Data ) 
	//{
	//	Vect3f l_Front = _pEntity->GetFront();
	//	l_Front.RotateY(mathUtils::Deg2Rad(30.f));
	//	l_Dir = Vect3f ( GetTarget().x + l_Front.x + l_Front.x  , GetTarget().y + _pEntity->GetHeight(), GetTarget().z + l_Front.z + l_Front.z );
	//	l_Dir.Normalize();
	//	l_Data  = CORE->GetPhysicsManager()->RaycastClosestActor( l_ObstaclePosition, l_Dir, l_Mask , s, _MaxDistance );

	//	if ( l_Data )
	//	{
	//		std::string l_Name = l_Data->GetName().c_str();
	//		if ( l_Name.compare(_pEntity->GetName()) != 0 ) 
	//		{
	//			LOGGER->AddNewLog( ELL_INFORMATION, "Colisió LATERAL -30º d'enemic %s amb el player: %s", _pEntity->GetName().c_str(), l_Data->GetName().c_str());
	//		}
	//	}
	//}

	if ( l_Data ) 
	{
		// Si encuentro un controller y es el objetivo (el player) retorno no colisión. Lo mismo si no es controler que deberíamos tratar el actor
		// Solo para testeo
		if ( l_Data->GetController() )
		{
			if ( l_Data->GetController()->GetPosition().x == GetTarget().x  &&  l_Data->GetController()->GetPosition().z == GetTarget().z )
			{
				//LOGGER->AddNewLog( ELL_INFORMATION, "Colisió d'enemic %s amb el player: %s", _pEntity->GetName().c_str(), l_Data->GetName().c_str());
				return NULL;
			}
		}
		else
		{
			// TODO: Caso de ser un CPHYSICACTOR
			return NULL;
		}

		// Miro que no sea él mismo
		std::string l_Name = l_Data->GetName().c_str();
		if ( l_Name.compare( _pEntity->GetName() ) != 0 ) 
		{
			LOGGER->AddNewLog( ELL_INFORMATION, "Colisió amb : %s", l_Data->GetName().c_str());
			return l_Data;
		}
		else
		{
			//LOGGER->AddNewLog( ELL_INFORMATION, "Colisiona %s amb ell mateix", l_Data->GetName().c_str());
			return NULL;
		}
	}
	
	//CHECKED_DELETE(l_Data);
	return NULL;
}
 
void CCollisionAvoidance::DrawRays( void )
{
	Mat44f mat;
	mat.SetIdentity();
	CRenderManager * l_RM = CORE->GetRenderManager();
	l_RM->SetTransform(mat);

	Vect3f _Ray;
	for ( size_t i = 0; i < m_Feelers.size(); i++ ) 
	{
		_Ray = m_Feelers[i];
		Vect3f l_InitialPosition = m_pEntity->GetPosition();
		l_InitialPosition.y += m_pEntity->GetHeight()/2;
		Vect3f l_FinalPosition = _Ray.GetNormalized() * m_DetectionRayLength;
		l_FinalPosition += l_InitialPosition;
		l_RM->DrawLine( l_InitialPosition, l_FinalPosition, colWHITE );
	}
}

const Vect3f CCollisionAvoidance::GetInitialPositionToThrowRay( void ) const
{
	Vect3f l_Pos = m_pEntity->GetPosition();
	l_Pos.y += m_pEntity->GetHeight()/2;
	
	Vect3f l_Front;
	l_Front = m_pEntity->GetFront();
	l_Front.Normalize();
	
	l_Pos = Vect3f ( l_Pos.x + l_Front.x, l_Pos.y, l_Pos.z);
	
	return l_Pos;
}

const Vect3f CCollisionAvoidance::GetDirectionRay( float _DegresOfRotation ) const
{
	Vect3f l_Front	= m_pEntity->GetFront();
	l_Front.Normalize();
		
	l_Front.RotateY(mathUtils::Deg2Rad(_DegresOfRotation));

	return (Vect3f(l_Front.x, 0.f, l_Front.z));
}


