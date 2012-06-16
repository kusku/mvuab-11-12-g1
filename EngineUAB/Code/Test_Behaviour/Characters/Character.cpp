#include "Character.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"

#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "States\IdleState.h"
#include "States\PursuitState.h"

#include "Properties/Properties.h"

#include "GameProcess.h"
#include "CharacterManager.h"

#include "Base.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity			( _Id )
	, CObject3D					( )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL )
	, m_pAnimationsStates		( NULL )
	, m_pProperties				( NULL )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
{
	// coloco la máquina de estados i el controler de física
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );

}

CCharacter::CCharacter( int _Id, const std::string &_Name )
	: CBaseGameEntity			( _Id )
	, CObject3D					( )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL ) 
	, m_pAnimationsStates		( NULL )
	, m_pProperties				( NULL )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
{
	// coloco la máquina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );

	SetName(_Name);
}

CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pLogicStateMachine );
	CHECKED_DELETE ( m_pGraphicStateMachine );
	// Amb lua no cal eliminar l'objecte. Lua ja se'n ocupa.
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
	//Create a dynamic Player     
	m_pPhysicUserDataJugador = new CPhysicUserData ( m_Name );
	m_pPhysicUserDataJugador->SetColor ( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint ( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController ( 1.f, 1.5f, 45.f, 0.1f, 0.5f, _Grup, m_pPhysicUserDataJugador );
	m_pController->SetPosition	( _InitialPosicion );
	m_pController->SetVisible	( true );
	
	CORE->GetPhysicsManager()->AddPhysicController( m_pController );
	
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

	if ( m_pCurrentAnimatedModel )
	{
		// Actualizamos el Yaw y lo asignamos al controler
		float l_Yaw = m_pCurrentAnimatedModel->GetYaw();
		m_pCurrentAnimatedModel->SetYaw( l_Yaw + mathUtils::Rad2Deg( m_pProperties->GetYaw() ) );
		m_pController->SetYaw( m_pProperties->GetYaw() );
	}

	return true;
}

void CCharacter::Update ( float _ElapsedTime )			
{ 
	m_pLogicStateMachine->Update( );
	m_pGraphicStateMachine->Update( );
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

//void CCharacter::MoveTo( const Vect3f &_Position, float _ElapsedTime )
//{
//	Vect3f l_LastPos = _Position;		// Ultima posición del player
//	Vect3f l_LastPositionNormalizated =  (l_LastPos - m_Position).Normalize();	// Ultima posición del player normalizda
//
//	Vect3f l_NewPosition = m_Position + GetFront() * _ElapsedTime * (m_pProperties->GetSpeed());
//	Vect3f l_NewPositionNormalizated = (l_LastPos - l_NewPosition).Normalize();
//	
//	if ( l_NewPositionNormalizated.Dot(l_LastPositionNormalizated) > 0  )
//	{
//		SetPosition(l_NewPosition);
//	}
//	else 
//	{
//		SetPosition(l_LastPos);
//	}
//
//	float l_RotationSpeed = m_pProperties->GetRotationSpeed();
//	if( IsPointAtLeft( l_LastPos ) ) {
//		AddYaw( -l_RotationSpeed * _ElapsedTime );
//		if ( !IsPointAtLeft(l_LastPos) ) 
//		{
//			float l_Yaw = l_LastPositionNormalizated.GetAngleY();
//			SetYaw(l_Yaw);
//		}
//	} 
//	else 
//	{
//		AddYaw( l_RotationSpeed * _ElapsedTime );
//		if ( IsPointAtLeft(l_LastPos) ) 
//		{
//			float l_Yaw = l_LastPositionNormalizated.GetAngleY();
//			SetYaw(l_Yaw);
//		}
//	}
//
//	this->GetController()->Move(m_Position, _ElapsedTime);
//	this->SetPosition(Vect3f ( GetController()->GetPosition().x, GetController()->GetPosition().y - GetController()->GetHeight() + 0.4f, GetController()->GetPosition().z));
//	this->GetAnimatedModel()->SetPosition( GetPosition() );
//}

void CCharacter::FaceTo( const Vect3f &_Position, float _ElapsedTime )
{
	Vect3f v = (_Position - m_Position);
	float l_RotationSpeed = m_pProperties->GetRotationSpeed();
	float l_back = v.Dot(GetFront());

	if(abs(l_back) < .9f)
	{
		return;
	}

	if ( l_back < 0 )
	{
		m_fYaw += (mathUtils::Deg2Rad(l_RotationSpeed) * _ElapsedTime);
	}
	else
	{
		m_fYaw += (-mathUtils::Deg2Rad(l_RotationSpeed) * _ElapsedTime);
	}
}

void CCharacter::MoveTo( const Vect3f &_Position, float _ElapsedTime )
{
	Vect2f pointA(_Position.x, _Position.z);
	Vect2f pointB(m_Position.x, m_Position.z);

	if(pointA.SqDistance(pointB) <= m_pProperties->GetAttackDistance())
	{
		FaceTo( _Position, _ElapsedTime );
		m_pController->SetYaw(m_fYaw);
		float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
		m_pCurrentAnimatedModel->SetYaw(-l_Yaw + 90.f );
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
	m_Position.y = m_Position.y - m_pController->GetHeight() + 0.4f;
	float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
	m_pCurrentAnimatedModel->SetYaw(-l_Yaw + 90.f );
	m_pCurrentAnimatedModel->SetPosition( m_Position );
}

//void CCharacter::FaceTo( const Vect3f &_Position, float _ElapsedTime )
//{
//	Vect3f l_LastPositionNormalizated =  (_Position - m_Position).Normalize();		// Ultima dirección a la posición del player normalitzada
//	
//	float l_RotationSpeed = m_pProperties->GetRotationSpeed();
//
//	if ( IsPointAtLeft( _Position ) ) {
//		AddYaw( -l_RotationSpeed * _ElapsedTime );
//		if( !IsPointAtLeft( _Position ) ) 
//		{
//			float l_Yaw = l_LastPositionNormalizated.GetAngleY();
//			SetYaw( l_Yaw );
//		}
//	} 
//	else 
//	{
//		AddYaw( l_RotationSpeed * _ElapsedTime );
//		if( IsPointAtLeft( _Position ) ) 
//		{
//			float l_Yaw = l_LastPositionNormalizated.GetAngleY ();
//			SetYaw( l_Yaw );
//		}
//	}
//}
//
//void CCharacter::MoveTo( const Vect3f &_Position, float _ElapsedTime )
//{
//	Vect3f l_LastPos = _Position;		// Ultima posició del player
//	Vect3f l_LastPositionNormalizated = (l_LastPos - GetPosition()).Normalize();		// Ultima posició del player normalitzada
//
//	Vect3f l_NewPosition				= GetPosition( ) + GetFront( ) * _ElapsedTime * m_pProperties->GetSpeed();
//	Vect3f l_NewPositionNormalizated	= ( l_LastPos - l_NewPosition ).Normalize();
//	if ( l_NewPositionNormalizated.Dot( l_LastPositionNormalizated ) > 0  )
//	{
//		SetPosition( l_NewPosition );
//	}
//	else {
//		SetPosition( l_LastPos );
//	}
//
//	FaceTo( _Position, _ElapsedTime );
//
//	//m_pController->SetYaw(m_fYaw);
//	MoveController(m_Position, _ElapsedTime);
//	
//	m_Position = m_pController->GetPosition();
//	m_Position.y = m_Position.y - m_pController->GetHeight() + 0.4f;
//	float l_Yaw = mathUtils::Rad2Deg(m_fYaw);
//	m_pCurrentAnimatedModel->SetYaw(m_fYaw);
//	m_pCurrentAnimatedModel->SetPosition( m_Position );
//}


bool CCharacter::IsPlayerDetected( void )
{
	CGameProcess* l_Process =  dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CCharacter* l_Player = l_Process->GetCharactersManager()->GetPlayer();

	Vect3f l_PlayerPosition = l_Player->GetPosition();
	Vect3f l_EnemyPosition  = this->GetPosition();

	Vect2f l_PointA(l_PlayerPosition.x, l_PlayerPosition.z);
	Vect2f l_PointB(l_EnemyPosition.x,  l_EnemyPosition.z);

	if (l_PointA.SqDistance(l_PointB) < this->GetProperties()->GetDetectionDistance())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CCharacter::AddLife( int _Life )								
{ 
	m_pProperties->SetLife( m_pProperties->GetLife() + _Life ); 
}

void CCharacter::RestLife( int _Life )								
{ 
	int l_Life = m_pProperties->GetLife() - _Life;
	if ( l_Life < 0 ) 
		l_Life = 0;

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

// -----------------------------------------
//				PROPERTIES
// -----------------------------------------
int CCharacter::GetAnimationId ( const std::string _AnimationName ) const
{
	CAnimatedCoreModel * l_Core =  m_pCurrentAnimatedModel->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( _AnimationName );
	return i;
}
