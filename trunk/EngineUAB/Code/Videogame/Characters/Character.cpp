#include "Character.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"
#include "Scripting\ScriptManager.h"

#include "Base.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// ----------------------------------------
//		WRAPPER PARA LUA
// ----------------------------------------
struct character_wrapper : CCharacter, luabind::wrap_base
{
	character_wrapper(int id)
		: CCharacter(id)
	{
	}

	virtual void Update(float _ElapsedTime)
	{
		call<void>("update", _ElapsedTime);
	}

	static void default_update(CCharacter *ptr, float _ElapsedTime)
	{
		ptr->CCharacter::Update(_ElapsedTime);
	}
};


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity			( _Id )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL )
	, m_pAnimationsStates		( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, CNamed					( )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
{
	// coloco la máquina de estados i el controler de física
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
   // m_pController			= new CPhysicController();
}

CCharacter::CCharacter( int _Id, const std::string &_Name )
	: CBaseGameEntity			( _Id )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL ) 
	, m_pAnimationsStates		( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, CNamed					( _Name )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
{
	// coloco la máquina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
  //  m_pController			= new CPhysicController();
}


CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pLogicStateMachine );
	CHECKED_DELETE ( m_pGraphicStateMachine );
	CHECKED_DELETE ( m_pController );
	CHECKED_DELETE ( m_pPhysicUserDataJugador );
	m_pCurrentAnimatedModel = NULL;
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CCharacter::Init ( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup )
{
	//Create a dynamic Player     
	m_pPhysicUserDataJugador = new CPhysicUserData ( m_Name );
	m_pPhysicUserDataJugador->SetColor ( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint ( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController ( 1.f, 1.5f, 45.f, 0.1f, 0.5f, _Grup, m_pPhysicUserDataJugador );
	m_pController->SetPosition	( Vect3f( 0.f, 3.5f, 0.f ) );
	m_pController->SetVisible	( true );
	
	CORE->GetPhysicsManager()->AddPhysicController( m_pController );

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

void CCharacter::RegisterMethods()
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CBaseGameEntity>("CBaseGameEntity")
	];

	module(state) [
		class_<CObject3D>("CObject3D")
	];

	module(state) [
		class_<CNamed>("CNamed")
	];

	module(state) [
		class_<CCharacter, character_wrapper, bases<CBaseGameEntity, CObject3D, CNamed>>("CCharacter")
			.def(constructor<int>())
			.def("update", &CCharacter::Update, &character_wrapper::default_update)
			//.def(constructor<int, const std::string&>())
	];
}