#include "Character.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"
#include "States\IdleState.h"
#include "States\PursuitState.h"

#include "Base.h"

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity	( _Id )
	, CNamed			( )
	, CObject3D			( )
	, m_Life			( 0 )
	, m_PrevPosition	( Vect3f(0.f, 0.f, 0.f) )
{
	// coloco la máquina de estados
	m_pPursuitState = new CPursuitState();
	m_pIdleState	= new CIdleState();
    m_pLogicStateMachine = new CStateMachine<CCharacter>( this );

	m_State = 0;
    m_pLogicStateMachine->SetCurrentState( m_pIdleState );
}

CCharacter::CCharacter( int _Id, const std::string &_Name )
	: CBaseGameEntity	( _Id )
	, CNamed			( _Name )
	, CObject3D			( )
	, m_Life			( 0 )
	, m_PrevPosition	( Vect3f(0.f, 0.f, 0.f) )
{
	// coloco la máquina de estados
    m_pLogicStateMachine = new CStateMachine<CCharacter>( this );

	m_pPursuitState = new CPursuitState();
	m_pIdleState	= new CIdleState();

	m_State = 0;
    m_pLogicStateMachine->SetCurrentState( m_pIdleState );
}


CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pLogicStateMachine );
	m_pCurrentAnimatedModel = NULL;

	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pIdleState );

	CHECKED_DELETE ( m_pController );
	CHECKED_DELETE ( m_pPhysicUserDataJugador );
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CCharacter::Init ( void )
{
	/*m_fPitch	= 0.f;
	m_fYaw		= 0.f;
	m_fRoll		= 0.f;*/
	
	//Create a dynamic Player     
	m_pPhysicUserDataJugador = new CPhysicUserData ( m_Name );
	m_pPhysicUserDataJugador->SetColor ( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint ( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController ( 1.f, 1.5f, 45.f, 0.1f, 0.5f, ::ECG_PERSONATGE, m_pPhysicUserDataJugador );
	m_pController->SetPosition	( Vect3f( 0.f, 3.5f, 0.f ) );
	m_pController->SetVisible	( true );
	
	CORE->GetPhysicsManager()->AddPhysicController( m_pController );

	m_pLogicStateMachine->SetCurrentState( m_pIdleState );

	return m_bIsOk;
}

void CCharacter::Update ( float _ElapsedTime )			
{ 
	Vect3f	l_PosActual	= m_pController->GetPosition();
	
	if ( m_PrevPosition != l_PosActual )
	{
		m_pCurrentAnimatedModel->SetPosition ( l_PosActual );
		if ( m_pLogicStateMachine->GetCurrentState() != m_pPursuitState )
		{
			m_pCurrentAnimatedModel->ClearCycle ( 0, 0.3f );
			m_pCurrentAnimatedModel->BlendCycle ( 1, 0.3f );
			m_pLogicStateMachine->ChangeState ( m_pPursuitState );
	//		//	//DISPACTH->DispatchStateMessage( 0.0f, 0, 0,
			m_State = 1;
		}
	}
	else
	{
		if ( m_pLogicStateMachine->GetCurrentState() != m_pIdleState )
		{
			m_pLogicStateMachine->ChangeState ( m_pIdleState );
			m_pCurrentAnimatedModel->ClearCycle ( 1, 0.3f );
			m_pCurrentAnimatedModel->BlendCycle ( 0, 0.3f );
			m_State = 0;
		}
		
	}

	m_PrevPosition = l_PosActual;

	m_pLogicStateMachine->Update( );
}

bool CCharacter::HandleMessage( const Telegram& _Msg )
{
	return m_pLogicStateMachine->HandleMessage( _Msg );
}

void CCharacter::MoveController(const Vect3f &_Dir, float _ElapsedTime)
{
	m_pController->Move( _Dir, _ElapsedTime );
}