#include "Character.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Core.h"
#include "Base.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// ----------------------------------------
//		WRAPPER PARA LUA
// ----------------------------------------
struct character_wrapper : CCharacter, luabind::wrap_base
{
	character_wrapper()
		: CCharacter()
	{
	}

	character_wrapper(const std::string &name)
		: CCharacter(name)
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
CCharacter::CCharacter()
	: CBaseGameEntity			( CBaseGameEntity::GetNextValidID() )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL )
	, m_pAnimationsStates		( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, CNamed					( )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
{
	// coloco la máquina de estados i el controler de física
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
   // m_pController			= new CPhysicController();

	m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*>(CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("caperucita1"));
}

CCharacter::CCharacter( const std::string &_Name )
	: CBaseGameEntity			( CBaseGameEntity::GetNextValidID() )
	, m_pLogicStateMachine		( NULL )
	, m_pGraphicStateMachine	( NULL )
	, m_pCurrentAnimatedModel	( NULL )
	, m_pController				( NULL ) 
	, m_pAnimationsStates		( NULL )
	, m_pPhysicUserDataJugador	( NULL )
	, CNamed					( _Name )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
{
	// coloco la máquina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
  //  m_pController			= new CPhysicController();

	m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*>(CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("caperucita1"));
}


CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pLogicStateMachine );
	CHECKED_DELETE ( m_pGraphicStateMachine );
	CHECKED_DELETE ( m_pPhysicUserDataJugador );
	CORE->GetPhysicsManager()->ReleasePhysicController( m_pController );
	m_pCurrentAnimatedModel = NULL;
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CCharacter::Init( const std::string &_Name, const Vect3f &_InitialPosition, ECollisionGroup _Group )
{
	//Create a dynamic Player     
	m_pPhysicUserDataJugador = new CPhysicUserData ( m_Name );
	m_pPhysicUserDataJugador->SetColor ( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint ( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController( 0.5f, 1.0f, 45.f, 0.1f, 0.5f, _Group, m_pPhysicUserDataJugador );
	m_pController->SetPosition( _InitialPosition );
	m_pController->SetVisible( true );
	
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
		class_<CCharacter, character_wrapper, bases<CBaseGameEntity, CObject3D, CNamed>>("CCharacter")
			.def(constructor<>())
			.def(constructor<const std::string&>())
			.def("init", &CCharacter::Init)
			.def("update", &CCharacter::Update, &character_wrapper::default_update)
			.property("physic_controller", &CCharacter::GetController)	
			.property("animated_model", &CCharacter::GetAnimatedModel)
			.property("locked", &CCharacter::GetLocked, &CCharacter::SetLocked)
	];
}