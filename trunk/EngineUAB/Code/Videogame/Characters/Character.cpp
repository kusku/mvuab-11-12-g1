#include "Character.h"
#include "CharacterManager.h"
#include "Properties/Properties.h"
#include "VideogameDefs.h"
#include "Utils\BoostRandomHelper.h"

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
#include "GameProcess.h"
#include "Cameras\ThPSCharacterCamera.h"

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
	// coloco la máquina de estados i el controler de física
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

	// coloco la máquina de estados
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
	// coloco la máquina de estados
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
	// En aquest cas volem alliberar el controller ja que aquest s'allibera si està mort però si no està mort cal fer-ho ara.
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

bool CCharacter::Initialize( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup )
{
	// Primero debemos adjuntar el modelo animado. Ojo! este nos da la posición
	CRenderableObjectsLayersManager *l_ROLayerManager = CORE->GetRenderableObjectsLayersManager();
	CRenderableObjectsManager *l_ROManager = l_ROLayerManager->GetResource("solid");
	CRenderableObject *l_RO = l_ROManager->GetInstance( m_pProperties->GetAnimationInstance() );

	if ( !l_RO ) 
	{
		l_ROManager->AddAnimatedMeshInstance( m_pProperties->GetName(), Vect3f (0.f, 0.f, 0.f ) );
	}
	else
	{
		m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*> (l_RO);
	}

	// Ahora creamos la parte física del controller del jugador
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
	m_pController->SetPosition( l_Position );
	m_pController->SetVisible( true );
	
	CORE->GetPhysicsManager()->AddPhysicController( m_pController, CAPSULE, _Grup );
	m_pController->Move( l_Position, 0.f );

	// Metemos el yaw y posición del modelo animado al controller
	if ( m_pCurrentAnimatedModel )
	{
		// Actualizamos el Yaw y lo asignamos al controler
		float l_Yaw = m_pCurrentAnimatedModel->GetYaw();
		m_pCurrentAnimatedModel->SetYaw( l_Yaw + mathUtils::Rad2Deg( m_pProperties->GetYaw() ) );
		m_pController->SetYaw( m_pProperties->GetYaw() );
		m_pCurrentAnimatedModel->SetPosition( l_Position );
	}
	
	this->SetName(_Name);
	this->SetLocked(m_pProperties->GetLocked());
	this->SetEnable ( m_pProperties->GetActive() );

	// Jordi 12/08/2012 -- Antes debemos inicializar el m_pSteeringEntity
	//MoveTo( l_Position, 0.0f );

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
		m_pSteeringEntity->SetPreviousPosition	( m_pProperties->GetPosition() );
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

		m_pSteeringEntity->SetYaw(m_pSteeringEntity->GetVelocity().GetAngleY());

		// Esto lo necesito para trabajar con los datos de movimiento y colisiones con físic
		m_pController->GetUserData()->SetSteeringEntity(m_pSteeringEntity);
		m_pSteeringEntity->SetController(m_pController);

		return true;
	}
}	

void CCharacter::Update ( float _ElapsedTime )			
{ 
	/*if( GetLocked() ) 
		return;*/
	
	m_pLogicStateMachine->Update( _ElapsedTime );
	m_pGraphicStateMachine->Update( _ElapsedTime );
}

void CCharacter::UpdatePlayer ( float _ElapsedTime )			
{ 
	// En Lua
}

void CCharacter::UpdateIA( float _ElapsedTime )			
{ 
	if( GetLocked() ) 
		return;
	
	Vect3f l_SteeringForce = m_pBehaviors->Update( _ElapsedTime, m_pSteeringEntity );
	l_SteeringForce.y = 0;

	// aceleración = fuerza/masa
	Vect3f l_Acceleration  = l_SteeringForce / m_pSteeringEntity->GetMass();
		
	// actualizamos la velocidad. Ya hemos comprobado en C++ su trucamiento con la max. velocidad
	m_pSteeringEntity->SetVelocity( m_pSteeringEntity->GetVelocity() + l_Acceleration * _ElapsedTime );
	m_pSteeringEntity->SetVelocity( Vect3f( m_pSteeringEntity->GetVelocity().x, 0, m_pSteeringEntity->GetVelocity().z ) );

	// nos aseguramos que el rabbit no excede de la velocidad máxima permitida
	Vect3f l_Velocity = m_pSteeringEntity->GetVelocity();
	l_Velocity.Truncate(m_pSteeringEntity->GetMaxSpeed());
	
	// actualizamos la posición
	m_pSteeringEntity->SetPosition( m_pSteeringEntity->GetPosition() + l_Velocity * _ElapsedTime );
		
	// Actualizamos el Heahing y Side de la entidad solo si esta tiene velocidad
	// print_logger ( 1, "CRabbit:updateIA->Squared_length : "..self.steering_entity.velocity:squared_length() )
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
		
	// Actualiza el heading del caracter para suabizarlo si está activado
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
	l_CanHandle &= m_pGraphicStateMachine->HandleMessage	( _Msg );

	return l_CanHandle;
}

void CCharacter::MoveController(const Vect3f &_Dir, float _ElapsedTime)
{
	m_pController->Move( _Dir, _ElapsedTime );
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
		l_DesiredYaw+=mathUtils::Deg2Rad(360.0f);
	if(m_fYaw<0.0f)
		m_fYaw+=mathUtils::Deg2Rad(360.0f);

	if((l_DesiredYaw-m_fYaw)>mathUtils::Deg2Rad(180.0f))
		l_DesiredYaw-=mathUtils::Deg2Rad(360.0f);
	else if((l_DesiredYaw-m_fYaw)<mathUtils::Deg2Rad(-180.0f))
		m_fYaw-=mathUtils::Deg2Rad(360.0f);
	
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
	//Vect2f pointA(_Position.x, _Position.z);
	//Vect2f pointB(m_Position.x, m_Position.z);

	//if ( pointA.SqDistance(pointB) <= m_pProperties->GetAttackDistance() )
	//{
	//	FaceTo( _Position, _ElapsedTime );
	//	m_pController->SetYaw(m_fYaw);
	//	Vect3f l_Position = Vect3f(0.0f, 0.0f, 0.0f);
	//	MoveController(l_Position, _ElapsedTime);

	//	m_Position = m_pController->GetPosition();
	//	m_Position.y = m_Position.y - m_pController->GetHeight() + m_pProperties->GetAnimationOffset();
	//	float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
	//	m_pCurrentAnimatedModel->SetYaw(l_Yaw/* + 90.f*/ );
	//	m_pCurrentAnimatedModel->SetPosition( m_Position );
	//	return;
	//}

	//FaceTo( _Position, _ElapsedTime );
	Vect3f l_Velocity (_Velocity.x, _Velocity.y, _Velocity.z);

	// Si queremos que la dirección sea más suave cojemos el heading del Smoother que és un average de los últimos headings
	if ( ( this->GetSteeringEntity()->isSmoothingOn() ) && ( this->GetSteeringEntity()->GetSpeed() != 0 ) )
	{
		float l_Speed = this->GetSteeringEntity()->GetSpeed();
		Vect3f v = this->GetSteeringEntity()->GetSmoothedHeading();
		v.Normalize();
		l_Velocity = v * l_Speed;
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
		Vect3f l_Position = Vect3f(0.0f, 0.0f, 0.0f);
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
	Vect3f l_Position = Vect3f(0.0f, 0.0f, 0.0f);
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

void CCharacter::AddLife( int _Life )								
{ 
	m_pProperties->SetLife( m_pProperties->GetLife() + _Life ); 
}

void CCharacter::RestLife( int _Life )								
{ 
	int l_Life = m_pProperties->GetLife() - _Life;
	if ( l_Life <= 0 ) 
	{
		l_Life = 0;
		m_bIsAlive = false;
	}
	m_pProperties->SetLife( l_Life ); 
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
	
	// std::string l_Name = l_Player->GetLogicFSM()->GetCurrentState()->GetName();
	
	/*if ( l_Player->GetLogicFSM()->isInState(l_Player->GetHitState()))
	{
		return false;
	}
	else
	{
		return true;
	}*/
		
	return true;
}

bool CCharacter::IsEnemyFocused( void )
{
	// Buscamos el enemigo más cercano en 360º ya que ataca el más cercano y no el que tenga de cara el player
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	
	Vect3f l_Front = l_Process->GetPlayerCamera()->GetDirection();
	float l_Distance = this->GetProperties()->GetDetectionDistance();
	CCharacter * l_EnemyDetected = l_Process->GetCharactersManager()->SearchTargetEnemy(l_Distance, e2PIf, l_Front);

	if ( !l_EnemyDetected ) 
	{
		// print_logger ( 1, "Enemy not detected" )
		return false;
	}
	else 
	{
		// print_logger ( 1, "Enemy detected :"..l_enemy_detected.name.." i el nom enemic : ".._enemy.name )
		if ( l_EnemyDetected->GetName() == this->GetName() ) 
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
	bool l_IsFocused 			= IsEnemyFocused();
	bool l_InsideAttackDistance	= IsPlayerInsideDistance(this->GetProperties()->GetAttackDistance());
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

	// miro si el player entra en distancia de detección
	float l_distance = l_positionA.Distance(l_positionB);
	if ( l_distance <= _DistanceToCheck  ) 
	{
		// print_logger (0, "Player detected!! go to player at: "..l_distance.." meters? metres de detecció enemic: ".._enemy.properties.detection_distance )
		return true;
	}
	else
	{
		// print_logger (0, "Player massa lluny!! : "..l_distance.." meters? metres de detecció enemic: ".._enemy.properties.detection_distance )
		return false;
	}
}

bool CCharacter::IsPlayerInsideImpactDistance( void )
{
	return IsPlayerInsideDistance(this->GetProperties()->GetImpactDistance());
}

bool CCharacter::IsPlayerDetected( void )
{
	return IsPlayerInsideDistance(this->GetProperties()->GetDetectionDistance());
}

bool CCharacter::IsPlayerChased( void )
{
	return IsPlayerInsideDistance(this->GetProperties()->GetChaseDistance());
}

bool CCharacter::IsEnemyPreparedToAttack( void )
{
	return IsPlayerInsideDistance(this->GetProperties()->GetPreparedAttackDistance());
}

bool CCharacter::IsEnemyAproximatedToAttack( void )
{
	return IsPlayerInsideDistance(this->GetProperties()->GetAproximationDistance());
}

// ---------------------------------------------------------------------------------------------------------------
// IsPlayerReached: Han pegado al Player!!
// ---------------------------------------------------------------------------------------------------------------
bool CCharacter::IsPlayerReached( void )
{
	return ( IsPlayerInsideDistance(this->GetProperties()->GetImpactDistance() )  && ( IsObstacleVisibleInAngle(GetPlayer(), 120) ) );
}

// ---------------------------------------------------------------------------------------------------------------
// GetPointInsideCameraFrustum: Obtiene una posición especifica 
// ---------------------------------------------------------------------------------------------------------------
Vect3f CCharacter::GetPointInsideCameraFrustum( void ) const
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	Vect3f	l_Front  = l_Process->GetPlayerCamera()->GetDirection();
	
	float l_Radi = this->GetProperties()->GetPreparedAttackDistance();

	CSteeringEntity * l_Entity	= l_Process->GetCharactersManager()->GetPlayer()->GetSteeringEntity();
	Vect3f l_Position = l_Entity->GetPosition();
	//local l_front 	= _CCharacter.steering_entity:get_front()
	float l_Height 	= l_Entity->GetHeight();
	l_Front.Normalize();
	l_Position = Vect3f( l_Position.x + l_Front.x * l_Radi , l_Position.y + l_Height, l_Position.z + l_Front.z * l_Radi );
	
	return l_Position;
}

// ---------------------------------------------------------------------------------------------------------------
// IsEnemyIntoCameraFrustum: Compruebo si el caracter està dentro del frustum de la camara especificado en grados
// ---------------------------------------------------------------------------------------------------------------
bool CCharacter::IsEnemyIntoCameraFrustum( float _RangeAngle, float _ElapsedTime )
{
	float l_AngleRad = mathUtils::Deg2Rad(_RangeAngle);
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	
	return (l_Process->GetCharactersManager()->IsEnemyVisibleInAngleFromCamera(this, l_AngleRad) );
}

// ---------------------------------------------------------------------------------------------------------------
// GoIntoCameraFrustum: Vamos al frustum de la cámara ya que no visualizamos el enemigo. Angulo en grados. 
// ---------------------------------------------------------------------------------------------------------------
void CCharacter::GoIntoCameraFrustum( float _RangeAngle, float _ElapsedTime )
{
	Vect3f l_PointToGo = GetPointInsideCameraFrustum();
	this->GetBehaviors()->GetSeek()->SetTarget(l_PointToGo);
	this->GetBehaviors()->SeekOn();
	this->FaceTo( this->GetPlayer()->GetPosition(), _ElapsedTime);
	this->MoveTo2(this->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
	LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s fuera del frustum de %f grados", this->GetName().c_str(), _RangeAngle);
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
//		LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s está dentro frustum de %f grados", this->GetName().c_str(), _RangeAngle);
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
// GetPositionToAttack: Obtiene una nueva posición para ubicarse para no entorpecer los demás y poder atacar
// ---------------------------------------------------------------------------------------------------------------
Vect3f CCharacter::GetPositionToAttack( void ) const
{
	// Pillamos un angulo cualquiera
	float l_NewAngle = static_cast<float> (BoostRandomHelper::GetFloat(0, 180));
	float l_NewAngleRadians = mathUtils::Deg2Rad(l_NewAngle);

	// Le añadimos este al anterior 
	//m_WanderOrientation += l_NewAngleRadians;

	//float l_TargetOrientation = m_WanderOrientation + _pEntity->GetYaw();
	//Vect3f l_TargetOrientationVector;
	//l_TargetOrientationVector.GetXZFromAngle(l_TargetOrientation);
	//l_TargetOrientationVector.Normalize();

	//Vect3f l_Front = _pEntity->GetFront();
	//l_Front.Normalize();
	//Vect3f l_Target = _pEntity->GetPosition() + m_WanderDistance * l_Front;

	//l_Target += m_WanderRadius * l_TargetOrientationVector;

	// Vector orientado del nuevo ángulo
	Vect3f l_TargetOrientationVector;
	l_TargetOrientationVector.GetXZFromAngle(l_NewAngleRadians);
	l_TargetOrientationVector.Normalize();

	CGameProcess * l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	CCharacter *l_Player = l_pProcess->GetPlayer();
	Vect3f l_CameraDir	= l_pProcess->GetPlayerCamera()->GetDirection();
	l_CameraDir.Normalize();
	
	// Esto me debería devolver la dirección de la cámera traladada a la posición del player
	Vect3f l_PlayerPosition = l_Player->GetPosition();
	Vect3f l_Target = l_PlayerPosition + this->GetProperties()->GetAttackDistance() * l_TargetOrientationVector;
	float l_Yaw = l_Target.GetAngleY();

	return l_Target;
}

// ---------------------------------------------------------------------------------------------------------------
// GetDistanceToPlayer: Obtiene la distancia al player. Se usa vector 2D ya que la y = 0
// ---------------------------------------------------------------------------------------------------------------
float CCharacter::GetDistanceToPlayer( void )
{
	Vect2f l_PositionA = Vect2f( this->GetPosition().x, this->GetPosition().z);
	Vect2f l_PositionB = Vect2f( this->GetPlayer()->GetPosition().x, this->GetPlayer()->GetPosition().z);
	
	float l_Distance = l_PositionA.Distance(l_PositionB);
	return l_Distance;
}

bool CCharacter::IsCollisionedWithSomething	( void )
{

	return false;
}

// ------------------------------------------------------------------------------------------------------------------
// IsObstacleVisibleInAngle: Devuelve si el obstaculo pasado está dentro de su frustum según el angulo especificado
//							Ángulo en grados
// ------------------------------------------------------------------------------------------------------------------
bool CCharacter::IsObstacleVisibleInAngle(CCharacter * _Obstacle, float _Angle)
{
	assert( _Obstacle );
	assert( _Angle > 0.f );
	
	float l_AngleRad = mathUtils::Deg2Rad(_Angle); 
	//Calculamos el vector entre el player y el enemigo
	Vect3f l_DirToObstacle = _Obstacle->GetPosition() - this->GetPosition();
	l_DirToObstacle.y = 0.f;
	l_DirToObstacle.Normalize(1.f);

	//Calculamos el ángulo entre los dos vectores
	//float l_Angle = l_DirPlayer.AngleWithVector(l_DirEnemy);		// Jordi : Jo tinc això...
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

void CCharacter::SetEnable( bool _Enable )
{
	m_pCurrentAnimatedModel->SetVisible(_Enable);
	m_pController->SetVisible(_Enable);
	m_pProperties->SetActive(_Enable);
	m_pProperties->SetVisible(_Enable);
	m_pController->SetActive(_Enable);
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