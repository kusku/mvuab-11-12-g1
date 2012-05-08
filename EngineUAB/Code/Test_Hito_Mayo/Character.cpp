#include "Character.h"
#include "PlayerControler.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "PursuitState.h"
#include "IdleState.h"

#include "Base.h"

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity	( _Id )
	, CNamed			( )
	, CObject3D			( )
	, m_Life			( 0 )
{
	// coloco la máquina de estados
    m_pStateMachine = new CStateMachine<CCharacter>( this );
    m_pControler = new CPlayerControler();

	m_State = 0;
    m_pStateMachine->SetCurrentState( m_pIdleState );

    /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE CHARACTER */
}

CCharacter::CCharacter( int _Id, const std::string &_Name )
	: CBaseGameEntity	( _Id )
	, CNamed			( _Name )
	, CObject3D			( )
	, m_Life			( 0 )
{
	// coloco la máquina de estados
    m_pStateMachine = new CStateMachine<CCharacter>( this );
    m_pControler	= new CPlayerControler();

	m_pPursuitState = new CPursuitState();
	m_pIdleState	= new CIdleState();

	m_State = 0;
    m_pStateMachine->SetCurrentState( m_pIdleState );

    /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE CHARACTER */
}


CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pStateMachine );
	CHECKED_DELETE ( m_pControler );
	m_pCurrentAnimatedModel = NULL;

	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pIdleState );
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CCharacter::Init ( void )
{
	m_fPitch	= 0.f;
	m_fYaw		= 0.f;
	m_fRoll		= 0.f;
	
	// Inicializamos la parte de física 
	m_bIsOk = m_pControler->Init( m_Name, Vect3f( 0.f, 3.5f, 0.f ) );
	
	return m_bIsOk;
}

void CCharacter::Update ( float _ElapsedTime )			
{ 
	Vect3f	m_PosAnterior = m_pControler->GetController()->GetPosition() ;
	m_pControler->Update( _ElapsedTime );
	Vect3f	m_PosActual	= m_pControler->GetController()->GetPosition();
	
	if ( m_PosAnterior != m_PosActual )
	{
		m_pCurrentAnimatedModel->SetPosition ( m_PosActual );
		if ( m_pStateMachine->GetCurrentState() != m_pPursuitState )
		{
			m_pCurrentAnimatedModel->ClearCycle ( 0, 0.3f );
			m_pCurrentAnimatedModel->BlendCycle ( 1, 0.3f );
			m_pStateMachine->ChangeState ( m_pPursuitState );
	//		//	//DISPACTH->DispatchStateMessage( 0.0f, 0, 0,
			m_State = 1;
		}
	}
	else
	{
		if ( m_pStateMachine->GetCurrentState() != m_pIdleState )
		{
			m_pStateMachine->ChangeState ( m_pIdleState );
			m_pCurrentAnimatedModel->ClearCycle ( 1, 0.3f );
			m_pCurrentAnimatedModel->BlendCycle ( 0, 0.3f );
			m_State = 0;
		}
		
	}
	//m_pStateMachine->Update( );
}

bool CCharacter::HandleMessage( const Telegram& _Msg )
{
	return m_pStateMachine->HandleMessage( _Msg );
}