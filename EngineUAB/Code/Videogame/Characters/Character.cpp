#include "Character.h"
#include "CharacterManager.h"
#include "Properties/Properties.h"
#include "Utils\BoostRandomHelper.h"
#include "Utils\Timer.h"
#include "Math\MathUtils.h"
#include "SoundManager.h"

#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\Seek.h"
#include "Steering Behaviors\Pursuit.h"
#include "Steering Behaviors\Arrive.h"

#include "StatesMachine\MessageDispatcher.h"

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"

#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"

#include "Scripting\ScriptManager.h"
#include "Cameras\ThPSCharacterCamera.h"

#include "Helpers\MathHelper.h"
#include "Utils\RecyclingArray.h"
#include "Speaker.h"
#include "EngineProcess.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter()
	: CBaseGameEntity			( CBaseGameEntity::GetNextValidID() )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL )
	, m_pAnimationsStates		( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, m_bLocked					( false )
	, m_pBehaviors				( NULL )
	, m_pSteeringEntity			( NULL )
	, m_bIsAlive				( true )
	, m_ReadyToAttack			( false )
	, m_PlayerHasBeenReached	( false )
	, m_pSpeaker				( NULL )
	, m_SpeakerName				( "" )
	, m_IsTired					( false )
{
	// coloco la m�quina de estados i el controler de f�sica
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
   // m_pController			= new CPhysicController();

	//m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*>(CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("caperucita1"));
}

CCharacter::CCharacter( const std::string &_Name )
	: CBaseGameEntity			( CBaseGameEntity::GetNextValidID() )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL ) 
	, m_pAnimationsStates		( NULL )
	, m_pProperties				( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, m_bLocked					( false )
	, m_pBehaviors				( NULL )
	, m_pSteeringEntity			( NULL )
	, m_bIsAlive				( true )
	, m_ReadyToAttack			( false )
	, m_PlayerHasBeenReached	( false )
	, m_pSpeaker				( NULL )
	, m_SpeakerName				( "" )
	, m_IsTired					( false )
{
	SetName ( _Name );

	// coloco la m�quina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
  //  m_pController			= new CPhysicController();

	//m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*>(CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("caperucita1"));
}

CCharacter::CCharacter(int _ID, const std::string &_Name)
	: CBaseGameEntity			( _ID )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL ) 
	, m_pAnimationsStates		( NULL )
	, m_pProperties				( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, m_bLocked					( false )
	, m_pBehaviors				( NULL )
	, m_pSteeringEntity			( NULL )
	, m_bIsAlive				( true )
	, m_ReadyToAttack			( false )
	, m_PlayerHasBeenReached	( false )
	, m_SpeakerName				( "" )
{
	SetName ( _Name );
	// coloco la m�quina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
}

CCharacter::~CCharacter()
{
	CHECKED_DELETE ( m_pBehaviors );
	CHECKED_DELETE ( m_pSteeringEntity );
	CHECKED_DELETE ( m_pLogicStateMachine );
	CHECKED_DELETE ( m_pGraphicStateMachine );
	// Amb lua no cal eliminar l'objecte. Lua ja se'n ocupa.
	// En aquest cas volem alliberar el controller ja que aquest s'allibera si est� mort per� si no est� mort cal fer-ho ara.
	if ( IsAlive() && CORE->GetPhysicsManager() )
		CORE->GetPhysicsManager()->ReleasePhysicController(m_pController);
	CHECKED_DELETE ( m_pController );
	CHECKED_DELETE ( m_pPhysicUserDataJugador );
	m_pCurrentAnimatedModel = NULL;
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
//bool CCharacter::Init( void )
//{
//	// Metodo y cosas a implementar en Lua
//	//if ( m_pCurrentAnimatedModel )
//	//{
//	//	// coloco el primer estado
//	//	m_pLogicStateMachine->SetCurrentState  ( m_pIdleState );
//	//	m_pGraphicStateMachine->SetCurrentState( m_pAnimationIdleState );
//	//}
//
//	return true;
//}

bool CCharacter::Initialize( const std::string &_Name, const std::string &_Core, const Vect3f &_InitialPosicion, ECollisionGroup _Grup )
{
	// Primero debemos adjuntar el modelo animado. Ojo! este nos da la posici�n
	CRenderableObjectsLayersManager *l_ROLayerManager = CORE->GetRenderableObjectsLayersManager();
	CRenderableObjectsManager *l_ROManager = l_ROLayerManager->GetResource("solid");
	CRenderableObject *l_RO = l_ROManager->GetInstance( m_pProperties->GetAnimationInstance() );

	if ( !l_RO ) 
	{
		CRenderableObject *l_RO = l_ROManager->AddAnimatedMeshInstance( m_pProperties->GetName(), _Core, m_pProperties->GetPosition() );
	}

	l_RO = l_ROManager->GetInstance( m_pProperties->GetAnimationInstance() );
	m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*> (l_RO);
	
	// Ahora creamos la parte f�sica del controller del jugador
	m_pPhysicUserDataJugador = new CPhysicUserData( _Name );
	m_pPhysicUserDataJugador->SetColor( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController( m_pProperties->GetWidthController(), m_pProperties->GetHeightController(), m_pProperties->GetSlopeController(), 
										   m_pProperties->GetSkinWidthController(), m_pProperties->GetStepOffsetController(), _Grup, m_pPhysicUserDataJugador );
	
	Vect3f l_Position; 
	if ( _InitialPosicion != NULL )
	{
		l_Position = _InitialPosicion;
	}
	else
	{
		l_Position = m_pCurrentAnimatedModel->GetPosition();
	}
	CORE->GetPhysicsManager()->AddPhysicController( m_pController, CAPSULE, _Grup );
	m_pController->SetPosition( l_Position );
	SetEnable(m_pProperties->GetActive());
	//m_pController->Move( m_pProperties->GetPosition(), 0.f );
	
	// Metemos el yaw y posici�n del modelo animado al controller
	if ( m_pCurrentAnimatedModel )
	{
		//// Actualizamos el Yaw y lo asignamos al controler
		//float l_Yaw = m_pCurrentAnimatedModel->GetYaw();
		//m_pCurrentAnimatedModel->SetYaw( l_Yaw + mathUtils::Rad2Deg( m_pProperties->GetYaw() ) );
		//m_pController->SetYaw( m_pProperties->GetYaw() );
		m_pCurrentAnimatedModel->SetPosition( l_Position );
		FaceToForPlayer(m_pProperties->GetYaw(), 0.f);
	}

	m_Name = _Name;
	m_bLocked = m_pProperties->GetLocked();
	
	// Jordi 12/08/2012 -- Antes debemos inicializar el m_pSteeringEntity
	//MoveTo( l_Position, 0.0f );

	RemoveActions();

	return true;
}

bool CCharacter::InitializeAI ( void )
{
	m_pBehaviors			= new CSteeringBehaviors( FUERZA_MAXIMA );
	m_pSteeringEntity		= new CSteeringEntity();

	if ( m_pBehaviors == NULL || m_pSteeringEntity == NULL )
	{
		return false;
	}
	else 
	{
		m_pSteeringEntity->SetPosition			( m_pProperties->GetPosition() );
		m_pSteeringEntity->SetPreviousPosition	( Vect3f(0,0,0) );
		m_pSteeringEntity->SetBoundingRadius	( m_pProperties->GetBoundingRadious() );
		m_pSteeringEntity->SetHeight			( m_pProperties->GetHeightController() );
		m_pSteeringEntity->SetName				( m_pProperties->GetName() );
		m_pSteeringEntity->SetMass 				( m_pProperties->GetMass() );
		m_pSteeringEntity->SetMaxForce			( m_pProperties->GetMaxForce() );
		m_pSteeringEntity->SetMaxSpeed			( m_pProperties->GetMaxSpeed() );
		m_pSteeringEntity->SetMaxRotation		( m_pProperties->GetMaxRotationSpeed() );
		m_pSteeringEntity->SetMaxAcceleration	( m_pProperties->GetMaxAcceleration() );
		m_pSteeringEntity->SetYaw				( m_pProperties->GetYaw() );
		
		m_pSteeringEntity->SetVelocity			( m_pProperties->GetVelocity() );
		if ( m_pSteeringEntity->GetVelocity().SquaredLength() > 0.00000001 )		
		{
			Vect3f v = m_pSteeringEntity->GetVelocity();
			v.Normalize();
			m_pSteeringEntity->SetHeading(v);
			m_pSteeringEntity->SetSide( v.GetPerpendicular() );
		}
		else
		{
			Vect3f v = m_pSteeringEntity->GetPosition();
			v.Normalize();
			m_pSteeringEntity->SetHeading(v);
			m_pSteeringEntity->SetSide( v.GetPerpendicular() );
		}

		//m_pSteeringEntity->SetYaw(m_pSteeringEntity->GetVelocity().GetAngleY());

		// Esto lo necesito para trabajar con los datos de movimiento y colisiones con f�sic
		m_pController->GetUserData()->SetSteeringEntity(m_pSteeringEntity);
		m_pSteeringEntity->SetController(m_pController);

		return true;
	}
}	

void CCharacter::Update ( float _ElapsedTime )			
{ 
	if( !m_pProperties->GetActive() ) 
		return;
	
	m_pLogicStateMachine->Update( _ElapsedTime );
	m_pGraphicStateMachine->Update( _ElapsedTime );
}

void CCharacter::UpdatePlayer ( float _ElapsedTime )			
{ 
	// En Lua
}

void CCharacter::UpdateIA( float _ElapsedTime )			
{ 
	if( m_bLocked ) 
		return;
	
	Vect3f l_SteeringForce = m_pBehaviors->Update( _ElapsedTime, m_pSteeringEntity );
	l_SteeringForce.y = 0;

	// aceleraci�n = fuerza/masa
	Vect3f l_Acceleration  = l_SteeringForce / m_pSteeringEntity->GetMass();
	float l_acc = l_Acceleration.Length();
	// actualizamos la velocidad. Ya hemos comprobado en C++ su trucamiento con la max. velocidad
	Vect3f l_FinalVelocity = m_pSteeringEntity->GetVelocity() + l_Acceleration * _ElapsedTime;
	l_FinalVelocity.y = 0;
	
	// nos aseguramos que el rabbit no excede de la velocidad m�xima permitida
	l_FinalVelocity.Truncate(m_pSteeringEntity->GetMaxSpeed());
	m_pSteeringEntity->SetVelocity( l_FinalVelocity );

	// actualizamos la posici�n
	m_pSteeringEntity->SetPosition( m_pSteeringEntity->GetPosition() + l_FinalVelocity * _ElapsedTime );
		
	// Actualizamos el Heahing y Side de la entidad solo si esta tiene velocidad
	if ( m_pSteeringEntity->GetVelocity().SquaredLength() > 0.00000001 ) 
	{
		// Ahora actualizamos el heading (Vector unitario velocidad) y su perpendicular
		Vect3f l_Vect = m_pSteeringEntity->GetVelocity();
		l_Vect.Normalize();
		m_pSteeringEntity->SetHeading( l_Vect );
		l_Vect = m_pSteeringEntity->GetHeading();
		m_pSteeringEntity->SetSide( l_Vect.GetPerpendicular() );
	}
	else
	{
		Vect3f l_Front = m_pSteeringEntity->GetFront();
		l_Front.Normalize();
		m_pSteeringEntity->SetHeading( l_Front );		// Ahora actualizamos el heading (Vector unitario velocidad) y su perpendicular
		Vect3f l_Vect = m_pSteeringEntity->GetHeading();
		m_pSteeringEntity->SetSide( l_Vect.GetPerpendicular() );
	}
		
	// Actualiza el heading del caracter para suabizarlo si est� activado
	m_pSteeringEntity->SmoothingOn();
	if ( m_pSteeringEntity->isSmoothingOn() )
	{
		//-- self.steering_entity.smoothing_heading = self.steering_entity.heading_smoother:update(self.steering_entity.heading);
		m_pSteeringEntity->SetSmoothedHeading( m_pSteeringEntity->GetHeadingSmoother()->Update( m_pSteeringEntity->GetHeading()));
	}
}

//-------------------------------------------------------------------------------------------------
// HandleMessage
//-------------------------------------------------------------------------------------------------
bool CCharacter::HandleMessage( const STelegram& _Msg, bool _Logic, bool _Graphic )
{
	if ( _Logic )
		m_pLogicStateMachine->HandleMessage( _Msg );
	
	if ( _Graphic )
		m_pGraphicStateMachine->HandleMessage( _Msg );

	return true;
}

//-------------------------------------------------------------------------------------------------
// HandleMessage
//-------------------------------------------------------------------------------------------------
bool CCharacter::HandleMessage( const STelegram& _Msg )
{
	bool l_CanHandle;
	l_CanHandle = m_pLogicStateMachine->HandleMessage		( _Msg );
	//l_CanHandle &= m_pGraphicStateMachine->HandleMessage	( _Msg );

	return l_CanHandle;
}

void CCharacter::MoveController(const Vect3f &_Dir, float _ElapsedTime)
{
	m_pController->Move( _Dir, _ElapsedTime );
}

void CCharacter::MoveCharacter(const Vect3f &_Pos)
{
	m_pController->SetPosition(_Pos);
	m_Position = m_pController->GetPosition();
	m_Position.y = m_Position.y - m_pController->GetHeight() + m_pProperties->GetAnimationOffset();
	m_pCurrentAnimatedModel->SetPosition( m_Position );
	m_pSteeringEntity->SetPosition( m_Position );
}

void CCharacter::FaceTo2( const Vect3f &_Position, float _ElapsedTime )
{
	//m_fYaw-=mathUtils::Deg2Rad(180.0f);
	if ( _Position.x == m_Position.x && _Position.z == m_Position.z )
		return;
	
	Vect3f l_ToTarget = _Position;
	l_ToTarget.y = .0f;
	l_ToTarget.Normalize();
	
	float l_DesiredYaw = l_ToTarget.GetAngleY();
	float l_RotationSpeed = m_pProperties->GetMaxRotationSpeed();

	if(l_DesiredYaw<0.0f)
		l_DesiredYaw+=mathUtils::Deg2Rad(360.0f);
	if(m_fYaw<0.0f)
		m_fYaw+=mathUtils::Deg2Rad(360.0f);

	if((l_DesiredYaw-m_fYaw)>mathUtils::Deg2Rad(180.0f))
		l_DesiredYaw-=mathUtils::Deg2Rad(360.0f);
	else if((l_DesiredYaw-m_fYaw)<mathUtils::Deg2Rad(-180.0f))
		m_fYaw-=mathUtils::Deg2Rad(360.0f);
	
	float l_Rot = mathUtils::Deg2Rad(l_RotationSpeed);
	if(l_DesiredYaw>m_fYaw)
		m_fYaw=mathUtils::Min(l_DesiredYaw, m_fYaw + l_Rot * _ElapsedTime);
	else 
	{
		m_fYaw=mathUtils::Max(l_DesiredYaw, m_fYaw - l_Rot * _ElapsedTime);
	}

	m_pController->SetYaw(m_fYaw);
	float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
	m_pCurrentAnimatedModel->SetYaw(l_Yaw /*+ 90.f */ );
	m_pSteeringEntity->SetYaw(l_Yaw);
}

void CCharacter::FaceToForPlayer( const Vect3f &_Position, float _ElapsedTime )
{
	//l_Yaw-=mathUtils::Deg2Rad(180.0f);
	if(_Position.x==m_Position.x && _Position.z==m_Position.z)
		return;

	//Calcula el �ngulo de giro
	Vect3f l_ToTarget = (_Position - m_Position);
	l_ToTarget.y=.0f;
	l_ToTarget.Normalize();

	Vect3f l_Front	= m_pCurrentAnimatedModel->GetFront();
	float l_fAngle	= l_ToTarget.Dot( l_Front );

	//Correci�n del �ngulo para que no de NaN en el arccos
	l_fAngle = Helper::LimitValue(l_fAngle, -1.f, 1.f);

	l_fAngle		= mathUtils::ACos<float>( l_fAngle );

	//Mira como tiene que girar el player
	bool l_bInside	= IsPointAtLeft( _Position, l_Front );
	float l_fYaw = m_pController->GetYaw();

	if( l_bInside )
	{
		l_fYaw	-= l_fAngle;
	}	
	else
	{
		l_fYaw	+= l_fAngle;
	}

	//Establece los valores para la ejecuci�n
	m_pController->SetYaw( l_fYaw );
	m_pSteeringEntity->SetYaw(l_fYaw);
	l_fYaw = -mathUtils::Rad2Deg(l_fYaw + FLOAT_PI_VALUE/2.f) + 180.f;
	m_pCurrentAnimatedModel->SetYaw(l_fYaw);
}

void CCharacter::FaceToForPlayer( float _DegreesAngle, float _ElapsedTime )
{
	float l_fAngle	= mathUtils::Deg2Rad(_DegreesAngle);

	//Correci�n del �ngulo para que no de NaN en el arccos
	//l_fAngle = Helper::LimitValue(l_fAngle, -1.f, 1.f);
	l_fAngle = mathUtils::ACos<float>( l_fAngle );

	//Mira como tiene que girar el player
	bool l_bInside	= IsPointAtLeft( GetPosition(), GetFront() );
	//float l_fYaw = m_pController->GetYaw();

	//if( l_bInside )
	//{
	//	l_fYaw	-= l_fAngle;
	//}	
	//else
	//{
	//	l_fYaw	+= l_fAngle;
	//}

	//Establece los valores para la ejecuci�n
	m_pController->SetYaw( l_fAngle );
	//m_pSteeringEntity->SetYaw(l_fAngle);
	l_fAngle = -mathUtils::Rad2Deg(l_fAngle + FLOAT_PI_VALUE/2.f) + 180.f;
	m_pCurrentAnimatedModel->SetYaw(l_fAngle);
}

void CCharacter::FaceTo( const Vect3f &_Position, float _ElapsedTime )
{
	//m_fYaw-=mathUtils::Deg2Rad(180.0f);
	if(_Position.x==m_Position.x && _Position.z==m_Position.z)
		return;

	Vect3f l_ToTarget = (_Position - m_Position);
	l_ToTarget.y=.0f;
	l_ToTarget.Normalize();

	float l_DesiredYaw = l_ToTarget.GetAngleY();
	float l_RotationSpeed = m_pProperties->GetMaxRotationSpeed();

	if(l_DesiredYaw<0.0f)
		l_DesiredYaw+= 2*FLOAT_PI_VALUE;
	if(m_fYaw<0.0f)
		m_fYaw+= 2*FLOAT_PI_VALUE;

	if((l_DesiredYaw-m_fYaw)>FLOAT_PI_VALUE)
		l_DesiredYaw-= 2*FLOAT_PI_VALUE;
	else if((l_DesiredYaw-m_fYaw)<-FLOAT_PI_VALUE)
		m_fYaw-= 2*FLOAT_PI_VALUE;
	
	if(l_DesiredYaw>m_fYaw)
		m_fYaw=mathUtils::Min(l_DesiredYaw, m_fYaw+mathUtils::Deg2Rad(l_RotationSpeed) * _ElapsedTime);
	else 
		m_fYaw=mathUtils::Max(l_DesiredYaw, m_fYaw-mathUtils::Deg2Rad(l_RotationSpeed) * _ElapsedTime);

	m_pController->SetYaw(m_fYaw);
	float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
	m_pCurrentAnimatedModel->SetYaw(l_Yaw /*+ 90.f */ );
	m_pSteeringEntity->SetYaw(m_fYaw);
}

void CCharacter::MoveTo2( const Vect3f &_Velocity, float _ElapsedTime )
{
	Vect3f l_Velocity (_Velocity.x, _Velocity.y, _Velocity.z);

	/*if( l_Velocity.SquaredLength() > 0.00000001f )
	{
		Vect3f l_Heading = m_pSteeringEntity->GetVelocity();
		l_Heading.Normalize(1.f);
			
		m_pSteeringEntity->SetHeading( l_Heading );
		m_pSteeringEntity->SetSide( l_Heading.GetPerpendicular() );
	}*/

	// Si queremos que la direcci�n sea m�s suave cojemos el heading del Smoother que �s un average de los �ltimos headings
	if ( ( m_pSteeringEntity->isSmoothingOn() ) && ( m_pSteeringEntity->GetSpeed() != 0 ) )
	{
		float l_Speed = m_pSteeringEntity->GetSpeed();
		Vect3f l_Vel = m_pSteeringEntity->GetSmoothedHeading();
		if( l_Vel.SquaredLength() > 0.00000001f )
		{
			l_Vel.Normalize();
			l_Velocity = l_Vel * l_Speed;
		}
	}

	MoveController(l_Velocity, _ElapsedTime);
	
	m_Position = m_pController->GetPosition();
	m_Position.y = m_Position.y - m_pController->GetHeight() + m_pProperties->GetAnimationOffset();
	//float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
	//m_pCurrentAnimatedModel->SetYaw(l_Yaw/* + 90.f*/ );
	m_pCurrentAnimatedModel->SetPosition( m_Position );
	m_pSteeringEntity->SetPosition( m_Position );
}

void CCharacter::MoveTo( const Vect3f &_Position, float _ElapsedTime )
{
	Vect2f pointA(_Position.x, _Position.z);
	Vect2f pointB(m_Position.x, m_Position.z);

	if(pointA.SqDistance(pointB) <= m_pProperties->GetAttackDistance())
	{
		FaceTo( _Position, _ElapsedTime );
		m_pController->SetYaw(m_fYaw);
		Vect3f l_Position = v3fZERO;
		MoveController(l_Position, _ElapsedTime);

		m_Position = m_pController->GetPosition();
		m_Position.y = m_Position.y - m_pController->GetHeight() + m_pProperties->GetAnimationOffset();
		float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
		m_pCurrentAnimatedModel->SetYaw(l_Yaw/* + 90.f*/ );
		m_pCurrentAnimatedModel->SetPosition( m_Position );
		return;
	}

	FaceTo( _Position, _ElapsedTime );

	Vect3f pointA2(_Position.x, 0, _Position.z);
	Vect3f pointB2(m_Position.x, 0, m_Position.z);
	Vect3f l_Position = v3fZERO;
	Vect3f l_Dir = (pointA2 - pointB2).Normalize();

	l_Position += l_Dir * m_pProperties->GetSpeed() * _ElapsedTime;

	m_pController->SetYaw(m_fYaw);
	MoveController(l_Position, _ElapsedTime);
	
	m_Position = m_pController->GetPosition();
	m_Position.y = m_Position.y - m_pController->GetHeight() + m_pProperties->GetAnimationOffset();
	float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
	m_pCurrentAnimatedModel->SetYaw(l_Yaw/* + 90.f*/ );
	m_pCurrentAnimatedModel->SetPosition( m_Position );
	m_pSteeringEntity->SetPosition ( m_Position );
}

// -----------------------------------------
//				METHODS
// -----------------------------------------

bool CCharacter::AddLife( int _Life )								
{ 
	int l_Life = m_pProperties->GetLife();
	int l_CurrentLife = m_pProperties->GetCurrentLife() + _Life;
	if( l_CurrentLife >=  l_Life )
	{
		l_CurrentLife = l_Life;
		m_pProperties->SetCurrentLife( l_CurrentLife ); 
		return false;
	}

	m_pProperties->SetCurrentLife( l_CurrentLife ); 

	return true;
}

void CCharacter::RestLife( int _Life )								
{ 
	int l_Life = m_pProperties->GetCurrentLife() - _Life;

	if ( l_Life <= 0 ) 
	{
		if( !m_pProperties->GetGodMode() )
		{
			l_Life = 0;
			m_bIsAlive = false;
		}
		else
		{
			l_Life = 1;
			m_bIsAlive = true;
		}
	}

	m_pProperties->SetCurrentLife( l_Life ); 
}

void CCharacter::AddSpeed( float _Speed )
{
	m_pProperties->SetSpeed( m_pProperties->GetSpeed() - _Speed ); 
}

void CCharacter::RestSpeed( float _Speed )
{
	float l_Speed = m_pProperties->GetSpeed() - _Speed;
	if ( l_Speed < 0 ) 
		l_Speed = 0;

	m_pProperties->SetSpeed( l_Speed ); 
}

void CCharacter::AddStrong( int _Strong )
{
	m_pProperties->SetStrong( m_pProperties->GetStrong() - _Strong ); 
}

void CCharacter::RestStrong( int _Strong )
{
	int l_Strong = m_pProperties->GetStrong() - _Strong;
	if ( l_Strong < 0 ) 
		l_Strong = 0;
	m_pProperties->SetStrong( l_Strong ); 
}

CPhysicUserData* CCharacter::ShootCollisionRay()
{
	SCollisionInfo l_Info;
	Vect3f l_Pos = m_Position;
	//Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
	Vect3f l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), 0.0, mathUtils::Sin<float>(m_fYaw));
	l_Dir.Normalize();

	l_Pos.y += m_pController->GetHeight()/2;
	l_Pos += l_Dir;

	int mask = 1 << ECG_PLAYER;
	mask |= 1 << ECG_DYNAMIC_OBJECTS;
	mask |= 1 << ECG_ESCENE;
	mask |= 1 << ECG_ENEMY;

	CPhysicUserData *userdata = CORE->GetPhysicsManager()->RaycastClosestActor(l_Pos, l_Dir, mask, l_Info);
	return userdata;
}

// TODO!!
bool CCharacter::IsPlayerReady( void )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CCharacter * l_Player = l_Process->GetPlayer();
	
	std::string l_Name = l_Player->GetLogicFSM()->GetCurrentState()->GetName();
	if (l_Name == "hit" )
	{
		return false;
	}
	else
	{
		return true;
	}
		
	return true;
}

bool CCharacter::IsEnemyFocused( void )
{
	// Buscamos el enemigo m�s cercano en 360� ya que ataca el m�s cercano y no el que tenga de cara el player
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	
	Vect3f l_Front = l_Process->GetPlayerCamera()->GetDirection();
	float l_Distance = m_pProperties->GetDetectionDistance();
	CCharacter * l_EnemyDetected = l_Process->GetCharactersManager()->SearchTargetEnemy(l_Distance, e2PIf, l_Front);

	if ( !l_EnemyDetected ) 
	{
		// print_logger ( 1, "Enemy not detected" )
		return false;
	}
	else 
	{
		// print_logger ( 1, "Enemy detected :"..l_enemy_detected.name.." i el nom enemic : ".._enemy.name )
		if ( l_EnemyDetected->GetName() == m_Name ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CCharacter::IsPlayerAtacable( void )
{
	//bool l_IsFocused 			= IsEnemyFocused();
	bool l_InsideAttackDistance	= IsPlayerInsideDistance(m_pProperties->GetAttackDistance());
	bool l_IsPlayerReady		= IsPlayerReady();

	//if ( l_IsFocused && l_InsideAttackDistance && l_IsPlayerReady )
	if ( l_InsideAttackDistance && l_IsPlayerReady )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CCharacter::IsPlayerInsideDistance( float _DistanceToCheck )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	Vect3f l_PlayerPosition = l_Process->GetPlayer()->GetPosition();
	Vect3f l_EnemyPosition  = GetPosition();

	Vect2f l_positionA = Vect2f( l_EnemyPosition.x, l_EnemyPosition.z );
	Vect2f l_positionB = Vect2f( l_PlayerPosition.x, l_PlayerPosition.z );

	// miro si el player entra en distancia de detecci�n
	float l_distance = l_positionA.Distance(l_positionB);
	if ( l_distance <= _DistanceToCheck  ) 
	{
		// print_logger (0, "Player detected!! go to player at: "..l_distance.." meters? metres de detecci� enemic: ".._enemy.properties.detection_distance )
		return true;
	}
	else
	{
		// print_logger (0, "Player massa lluny!! : "..l_distance.." meters? metres de detecci� enemic: ".._enemy.properties.detection_distance )
		return false;
	}
}

bool CCharacter::IsPlayerInsideImpactDistance( void )
{
	return IsPlayerInsideDistance(m_pProperties->GetImpactDistance());
}

bool CCharacter::IsPlayerDetected( void )
{
	return IsPlayerInsideDistance(m_pProperties->GetDetectionDistance());
}

bool CCharacter::IsPlayerChased( void )
{
	return IsPlayerInsideDistance(m_pProperties->GetChaseDistance());
}

bool CCharacter::IsEnemyPreparedToAttack( void )
{
	return IsPlayerInsideDistance(m_pProperties->GetPreparedAttackDistance());
}

bool CCharacter::IsEnemyAproximatedToAttack( void )
{
	return IsPlayerInsideDistance(m_pProperties->GetAproximationDistance());
}

// ---------------------------------------------------------------------------------------------------------------
// IsPlayerReached: Han pegado al Player!!
// ---------------------------------------------------------------------------------------------------------------
bool CCharacter::IsPlayerReached( void )
{
	return ( IsPlayerInsideDistance(m_pProperties->GetImpactDistance() )  && ( IsObstacleVisibleInAngle(GetPlayer(), 150) ) );
}

// ---------------------------------------------------------------------------------------------------------------
// GetPointInsideCameraFrustum: Obtiene una posici�n especifica dentro de la distancia de preparaci�n y segun 
//							el �ngulo en grados
// ---------------------------------------------------------------------------------------------------------------
Vect3f CCharacter::GetPointInsideCameraFrustum( float _VisionAngle ) const
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	
	// Calculamos el front y lo rotamos segun un random 
	Vect3f	l_Front  = l_Process->GetPlayerCamera()->GetDirection();
	float l_Angle = BoostRandomHelper::GetFloat(-_VisionAngle/2,_VisionAngle/2); 
	l_Front.RotateY(mathUtils::Deg2Rad(l_Angle));

	// Obtenemos la m�xima distancia donde ir, entre la distancia de ataque y la preparaci�n
	float l_Radi = BoostRandomHelper::GetFloat(m_pProperties->GetAttackDistance(),m_pProperties->GetPreparedAttackDistance());  

	// Calculamos la posici�n final
	CSteeringEntity * l_Entity	= l_Process->GetCharactersManager()->GetPlayer()->GetSteeringEntity();
	Vect3f l_Position = l_Entity->GetPosition();
	float l_Height 	= l_Entity->GetHeight();
	l_Front.Normalize();
	l_Position = Vect3f( l_Position.x + l_Front.x * l_Radi , l_Position.y + l_Height, l_Position.z + l_Front.z * l_Radi );
	
	int l_Mask = 1 << ECG_LIMITS;
	l_Mask = 1 << ECG_STATIC_OBJECTS;
	l_Mask = 1 << ECG_DYNAMIC_OBJECTS;
	// Si la posici�n no est� dentro de los limites entonces retornamos y mantenemos la misma posicion
	if ( IsPointTouchingGroup(l_Position, l_Mask, 0.5f ) )
	{
		l_Position = m_Position; 
	}
	
	return l_Position;
}

// ---------------------------------------------------------------------------------------------------------------
// IsPointInsideLimits : Obtiene si una posici�n especifica est� dentro de los muros l�mite
// ---------------------------------------------------------------------------------------------------------------
bool CCharacter::IsPointTouchingGroup( const Vect3f &_Position, int _MasK, float _Radius ) const
{
	std::vector<CPhysicUserData*> l_UserDatas;			// Permite almacenar aquellos l�mites en zona de detecci�n de la posici�n 
	
	// Calculamos l�mites vecinos a una posici�n 
	CORE->GetPhysicsManager()->OverlapSphereActor( _Radius, _Position, l_UserDatas, _MasK );

	// Si encontramos colisiones con l�mites ya no aceptamos la posici�n
	if ( l_UserDatas.size() > 0 )
	{
		LOGGER->AddNewLog(ELL_WARNING, "IsPointTouchingLimits -> colisiona con : %s", l_UserDatas[0]->GetName().c_str() );
		return true;
	}	

	return false;
}

// ---------------------------------------------------------------------------------------------------------------
// IsEnemyIntoCameraFrustum: Compruebo si el caracter est� dentro del frustum de la camara especificado en grados
// ---------------------------------------------------------------------------------------------------------------
bool CCharacter::IsEnemyIntoCameraFrustum( float _RangeAngle, float _ElapsedTime )
{
	float l_AngleRad = mathUtils::Deg2Rad(_RangeAngle);
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	
	return (l_Process->GetCharactersManager()->IsEnemyVisibleInAngleFromCamera(this, l_AngleRad) );
}

// ---------------------------------------------------------------------------------------------------------------
// GoIntoCameraFrustum: Vamos al frustum de la c�mara ya que no visualizamos el enemigo. Angulo en grados. 
// ---------------------------------------------------------------------------------------------------------------
void CCharacter::GoIntoCameraFrustum( float _RangeAngle, float _ElapsedTime )
{
	Vect3f l_PointToGo = GetPointInsideCameraFrustum(_RangeAngle);
	m_pBehaviors->GetSeek()->SetTarget(l_PointToGo);
	m_pBehaviors->SeekOn();
	FaceTo( GetPlayer()->GetPosition(), _ElapsedTime);
	MoveTo2(m_pSteeringEntity->GetVelocity(), _ElapsedTime);
	LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s fuera del frustum de %f grados", m_Name.c_str(), _RangeAngle);
}

//void CCharacter::GoIntoCameraFrustum( float _RangeAngle, float _ElapsedTime )
//{
//	float l_AngleRad = mathUtils::Deg2Rad(_RangeAngle);
//	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
//	//Vect3f	l_Front  = l_Process->GetPlayerCamera()->GetDirection();
//	
//	if ( l_Process->GetCharactersManager()->IsEnemyVisibleInAngleFromCamera(this, l_AngleRad ) )
//	{
//		this->GetBehaviors()->GetSeek()->SetTarget(this->GetPlayer()->GetPosition());
//		this->GetBehaviors()->SeekOn();
//		this->FaceTo( this->GetPlayer()->GetPosition(), _ElapsedTime);
//		this->MoveTo2(this->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
//		LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s est� dentro frustum de %f grados", this->GetName().c_str(), _RangeAngle);
//		return false;
//	}
//	else
//	{
//		//this->GetBehaviors()->PursuitOff();
//		Vect3f l_PointToGo = GetPointInsideCameraFrustum();
//		this->GetBehaviors()->GetSeek()->SetTarget(l_PointToGo);
//		this->GetBehaviors()->SeekOn();
//		this->FaceTo( this->GetPlayer()->GetPosition(), _ElapsedTime);
//		this->MoveTo2(this->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
//		LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s fuera del frustum de %f grados", this->GetName().c_str(), _RangeAngle);
//	}
//}


// ---------------------------------------------------------------------------------------------------------------
// GetPositionToAttack: Obtiene una nueva posici�n para ubicarse para no entorpecer los dem�s y poder atacar
// ---------------------------------------------------------------------------------------------------------------
Vect3f CCharacter::GetPositionToAttack( void ) const
{
	// Pillamos un angulo cualquiera
	float l_NewAngle = static_cast<float> (BoostRandomHelper::GetFloat(0, 180));
	float l_NewAngleRadians = mathUtils::Deg2Rad(l_NewAngle);

	// Le a�adimos este al anterior 
	//m_WanderOrientation += l_NewAngleRadians;

	//float l_TargetOrientation = m_WanderOrientation + _pEntity->GetYaw();
	//Vect3f l_TargetOrientationVector;
	//l_TargetOrientationVector.GetXZFromAngle(l_TargetOrientation);
	//l_TargetOrientationVector.Normalize();

	//Vect3f l_Front = _pEntity->GetFront();
	//l_Front.Normalize();
	//Vect3f l_Target = _pEntity->GetPosition() + m_WanderDistance * l_Front;

	//l_Target += m_WanderRadius * l_TargetOrientationVector;

	// Vector orientado del nuevo �ngulo
	Vect3f l_TargetOrientationVector;
	l_TargetOrientationVector.GetXZFromAngle(l_NewAngleRadians);
	l_TargetOrientationVector.Normalize();

	CGameProcess * l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	CCharacter *l_Player = l_pProcess->GetPlayer();
	Vect3f l_CameraDir	= l_pProcess->GetPlayerCamera()->GetDirection();
	l_CameraDir.Normalize();
	
	// Esto me deber�a devolver la direcci�n de la c�mera traladada a la posici�n del player
	Vect3f l_PlayerPosition = l_Player->GetPosition();
	Vect3f l_Target = l_PlayerPosition + m_pProperties->GetAttackDistance() * l_TargetOrientationVector;
	float l_Yaw = l_Target.GetAngleY();

	return l_Target;
}

// ---------------------------------------------------------------------------------------------------------------
// GetDistanceToPlayer: Obtiene la distancia al player. Se usa vector 2D ya que la y = 0
// ---------------------------------------------------------------------------------------------------------------
float CCharacter::GetDistanceToPlayer( void )
{
	Vect2f l_PositionA = Vect2f( m_Position.x, m_Position.z);
	Vect2f l_PositionB = Vect2f( GetPlayer()->GetPosition().x, GetPlayer()->GetPosition().z);
	
	float l_Distance = l_PositionA.Distance(l_PositionB);
	return l_Distance;
}

// ---------------------------------------------------------------------------------------------------------------
// GetDistanceToPointIn2D: Obtiene la distancia al punto. Se usa vector 2D ya que la y = 0
// ---------------------------------------------------------------------------------------------------------------
float CCharacter::GetDistanceToPointIn2D( const Vect3f & _Position )
{
	Vect2f l_PositionA = Vect2f( m_Position.x, m_Position.z);
	Vect2f l_PositionB = Vect2f( _Position.x, _Position.z);
	
	float l_Distance = l_PositionA.Distance(l_PositionB);
	return l_Distance;
}

bool CCharacter::IsCollisionedWithSomething	( void )
{

	return false;
}

// ------------------------------------------------------------------------------------------------------------------
// IsObstacleVisibleInAngle: Devuelve si el obstaculo pasado est� dentro de su frustum seg�n el angulo especificado
//							�ngulo en grados
// ------------------------------------------------------------------------------------------------------------------
bool CCharacter::IsObstacleVisibleInAngle(CCharacter * _Obstacle, float _Angle)
{
	assert( _Obstacle );
	assert( _Angle > 0.f );
	
	float l_AngleRad = mathUtils::Deg2Rad(_Angle); 
	//Calculamos el vector entre el player y el enemigo
	Vect3f l_DirToObstacle = _Obstacle->GetPosition() - m_Position;
	l_DirToObstacle.y = 0.f;
	l_DirToObstacle.Normalize(1.f);

	//Calculamos el �ngulo entre los dos vectores
	//float l_Angle = l_DirPlayer.AngleWithVector(l_DirEnemy);		// Jordi : Jo tinc aix�...
	float l_Angle = GetFront().Dot(l_DirToObstacle);
	l_AngleRad = mathUtils::ACos(l_AngleRad);

	if( l_AngleRad > _Angle )
	{
		return false;
	}

	return true;
}

// -----------------------------------------
//				PROPERTIES
// -----------------------------------------
int CCharacter::GetAnimationID( const std::string &_AnimationName )
{
	CAnimatedCoreModel * l_Core =  m_pCurrentAnimatedModel->GetAnimatedCoreModel();
	return l_Core->GetCoreModel()->getCoreAnimationId( _AnimationName );
}

void CCharacter::Appearance( void )
{
	// Si se habilitat aparecen con part�culas y sonido
	Vect3f l_Pos1 = GetPosition();
	Vect3f l_Pos2 = GetPlayer()->GetPosition();
	Vect3f l_RelativePosition = l_Pos2 - l_Pos1;
	l_RelativePosition.Normalize();
	Vect3f l_Pos = l_Pos1 + l_RelativePosition;
	l_Pos.y = l_Pos1.y;

	if ( m_Type != WOLF )
	{
		CORE->GetParticleEmitterManager()->GetResource("Twister")->GetParticleEmitterInstance(GetName() + "_Twister")->SetPosition(l_Pos);
		CORE->GetParticleEmitterManager()->GetResource("Twister")->GetParticleEmitterInstance(GetName() + "_Twister")->EjectParticles();
		CORE->GetSoundManager()->PlayEvent( GetSpeakerName(), "Play_EFX_51467_missile_explosion" );
	}
	else
	{
		CORE->GetParticleEmitterManager()->GetResource("WolfTwister")->GetParticleEmitterInstance(GetName() + "_WolfTwister")->SetPosition(l_Pos);
		CORE->GetParticleEmitterManager()->GetResource("WolfTwister")->GetParticleEmitterInstance(GetName() + "_WolfTwister")->EjectParticles();
		CORE->GetParticleEmitterManager()->GetResource("WolfTwisterExplosion")->GetParticleEmitterInstance(GetName() + "_WolfTwisterExplosion")->SetPosition(l_Pos);
		CORE->GetParticleEmitterManager()->GetResource("WolfTwisterExplosion")->GetParticleEmitterInstance(GetName() + "_WolfTwisterExplosion")->EjectParticles();
		CORE->GetSoundManager()->PlayEvent( GetSpeakerName(), "Play_EFX_51429_volcano_eruption" );
	}
}

void CCharacter::SetEnable( bool _Enable )
{
	m_pController->SetCollision(_Enable);
	m_pController->SetVisible(_Enable);
	m_pProperties->SetVisible(_Enable);
	m_pProperties->SetActive(_Enable);
	m_pCurrentAnimatedModel->SetVisible(_Enable);
	SetLocked(!_Enable);
	if (!_Enable & !IsAlive())
	{
		CORE->GetPhysicsManager()->ReleasePhysicController(m_pController);
		//CHECKED_DELETE(m_pController);
	}
}

CCharacter* CCharacter::GetPlayer( void )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CCharacter* l_pCharacter = l_Process->GetCharactersManager()->GetPlayer();
	return l_pCharacter;
}

CState<CCharacter>* CCharacter::GetLogicState( const std::string &_State )
{
	TStateMap::iterator l_pState = m_LogicStatesMap.find(_State);

	if(l_pState != m_LogicStatesMap.end())
	{
		return l_pState->second;
	}

	return NULL;
}

CState<CCharacter>* CCharacter::GetAnimationState( const std::string &_State )
{
	TStateMap::iterator l_pState = m_AnimationStatesMap.find(_State);

	if(l_pState != m_AnimationStatesMap.end())
	{
		return l_pState->second;
	}

	return NULL;
}

void CCharacter::SetSoundsOff( void )
{
	return;
}

void CCharacter::RemoveActions()
{

}