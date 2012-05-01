#include "Character.h"
#include "Base.h"

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity	( _Id )
	, m_Life			( 0 )
{
	// coloco la máquina de estados
    m_pStateMachine = new CStateMachine<CCharacter>( this );
    
    m_pStateMachine->SetCurrentState( IDLESTATE );

    /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE CHARACTER */
}


CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pStateMachine );
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
void CCharacter::Update ( float _ElapsedTime )			
{ 
	m_pStateMachine->Update();
}

bool CCharacter::HandleMessage( const Telegram& _Msg )
{
	return m_pStateMachine->HandleMessage( _Msg );
}