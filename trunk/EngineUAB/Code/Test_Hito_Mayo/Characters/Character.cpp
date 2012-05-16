#include "Character.h"
#include "PlayerControler.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "../States/PursuitState.h"
#include "../States/IdleState.h"

//#include "Cameras\Camera.h"
#include "Base.h"
#include "Core.h"


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity			( _Id )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	
{
	// coloco la máquina de estados i el controler de física
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
    m_pController			= new CPlayerControler();
	
	/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE CHARACTER */
}

CCharacter::CCharacter( int _Id, const std::string &_Name )
	: CBaseGameEntity			( _Id )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
{
	// coloco la máquina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
    m_pController			= new CPlayerControler();

	/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE CHARACTER */
}

CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pLogicStateMachine );
	CHECKED_DELETE ( m_pGraphicStateMachine );
	CHECKED_DELETE ( m_pController );
	m_pCurrentAnimatedModel = NULL;
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CCharacter::Init ( const std::string &_Name, const Vect3f &_InitialPosicion )
{
	// Inicializamos la parte de física 
	return m_pController->Init( _Name, _InitialPosicion );
}

void CCharacter::Update ( float _ElapsedTime )			
{
	Vect3f	l_PosActual	= m_pController->GetPosition();
	
	if ( m_PrevPosition != l_PosActual )
	{
		m_pCurrentAnimatedModel->SetPosition ( l_PosActual );
		
	}
	else
	{
		
	}

	//m_PrevPosition = l_PosActual;
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