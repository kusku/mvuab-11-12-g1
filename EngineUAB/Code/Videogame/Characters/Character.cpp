#include "Character.h"
#include "CharacterWrapper.h"
#include "Properties/Properties.h"
#include "VideogameDefs.h"

#include "Steering Behaviours\SteeringBehaviours.h"
#include "Steering Behaviours\SteeringBehavioursDefs.h"
#include "Steering Behaviours\SteeringEntity.h"
#include "Steering Behaviours\Seek.h"
#include "Steering Behaviours\Pursuit.h"
#include "Steering Behaviours\Arrive.h"

#include "StatesMachine\MessageDispatcher.h"

#include "Math/Vector3.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"

#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"

#include "Scripting\ScriptManager.h"
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
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
	, m_pBehaviours				( NULL )
	, m_pSteeringEntity			( NULL )
	, m_bIsAlive				( true )
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
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
	, m_pBehaviours				( NULL )
	, m_pSteeringEntity			( NULL )
	, m_bIsAlive				( true )
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
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
	, m_pBehaviours				( NULL )
	, m_pSteeringEntity			( NULL )
	, m_bIsAlive				( true )
{
	SetName ( _Name );
	// coloco la máquina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
}

CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pBehaviours );
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
bool CCharacter::Init( void )
{
	// Metodo y cosas a implementar en Lua
	//if ( m_pCurrentAnimatedModel )
	//{
	//	// coloco el primer estado
	//	m_pLogicStateMachine->SetCurrentState  ( m_pIdleState );
	//	m_pGraphicStateMachine->SetCurrentState( m_pAnimationIdleState );
	//}

	return true;
}

bool CCharacter::Initialize ( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup )
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
	
	CORE->GetPhysicsManager()->AddPhysicController( m_pController );
	m_pController->Move( l_Position, 0.f );

	// Metemos el yaw y posición del modelo animado al controller
	if ( m_pCurrentAnimatedModel )
	{
		// Actualizamos el Yaw y lo asignamos al controler
		float l_Yaw = m_pCurrentAnimatedModel->GetYaw();
		m_pCurrentAnimatedModel->SetYaw( l_Yaw + mathUtils::Rad2Deg( m_pProperties->GetYaw() ) );
		m_pController->SetYaw( m_pProperties->GetYaw() );
	}
	
	this->SetName(_Name);
	this->SetEnable ( m_pProperties->GetActive() );

	MoveTo( l_Position, 0.0f );

	return true;
}

bool CCharacter::InitializeAI ( void )
{
	m_pBehaviours			= new CSteeringBehaviours( FUERZA_MAXIMA );
	m_pSteeringEntity		= new CSteeringEntity();

	if ( m_pBehaviours == NULL || m_pSteeringEntity == NULL )
	{
		return false;
	}
	else 
	{
		m_pSteeringEntity->SetBoundingRadius ( m_pProperties->GetBoundingRadious() );
		m_pSteeringEntity->SetMaxSpeed ( m_pProperties->GetBoundingRadious() );

		m_pBehaviours->AddBehavior( new CSeek() );
		m_pBehaviours->AddBehavior( new CPursuit(::normal, 50.f) );
		m_pBehaviours->AddBehavior( new CArrive(::normal, 50.f) );

		return true;
	}
}	

void CCharacter::Update ( float _ElapsedTime )			
{ 
	/*m_pLogicStateMachine->Update( );
	m_pGraphicStateMachine->Update( );*/
}

//-------------------------------------------------------------------------------------------------
// HandleMessage
//-------------------------------------------------------------------------------------------------
bool CCharacter::HandleMessage( const Telegram& _Msg, bool _Logic, bool _Graphic )
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
bool CCharacter::HandleMessage( const Telegram& _Msg )
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

void CCharacter::FaceTo( const Vect3f &_Position, float _ElapsedTime )
{
	//m_fYaw-=mathUtils::Deg2Rad(180.0f);
	if(_Position.x==m_Position.x && _Position.z==m_Position.z)
		return;
	Vect3f v = (_Position - m_Position);
	v.y=.0f;
	v.Normalize();
	float l_DesiredYaw=v.GetAngleY();
	float l_RotationSpeed = m_pProperties->GetRotationSpeed();

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
}

// -----------------------------------------
//				PROPERTIES
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
		SetAlive(false);
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

	int mask = 1 << ECG_PERSONATGE;
	mask |= 1 << ECG_OBJECTES_DINAMICS;
	mask |= 1 << ECG_ESCENARI;
	mask |= 1 << ECG_ENEMICS;

	CPhysicUserData *userdata = CORE->GetPhysicsManager()->RaycastClosestActor(l_Pos, l_Dir, mask, l_Info);
	return userdata;
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
	m_pProperties->SetActive(_Enable);
	m_pProperties->SetVisible(_Enable);
	m_pCurrentAnimatedModel->SetVisible(_Enable);
	m_pController->SetActive(_Enable);
	m_pController->SetVisible(_Enable);
	if (!_Enable & !IsAlive())
	{
		CORE->GetPhysicsManager()->ReleasePhysicController(m_pController);
		//CHECKED_DELETE(m_pController);
	}
}