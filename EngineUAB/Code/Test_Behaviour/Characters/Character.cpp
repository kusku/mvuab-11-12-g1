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

#include "Base.h"

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CCharacter::CCharacter( int _Id )
	: CBaseGameEntity			( _Id )
	, CNamed					( )
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
	// coloco la m�quina de estados i el controler de f�sica
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );

}

CCharacter::CCharacter( int _Id, const std::string &_Name )
	: CBaseGameEntity			( _Id )
	, CNamed					( _Name )
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
	// coloco la m�quina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );

}

CCharacter::~CCharacter( void )
{
	CHECKED_DELETE ( m_pLogicStateMachine );
	CHECKED_DELETE ( m_pGraphicStateMachine );
	// Amb lua no cal eliminar l'objecte. Lua ja se'n ocupa.
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



// -----------------------------------------
//				PROPERTIES
// -----------------------------------------
int CCharacter::GetAnimationId ( const std::string _AnimationName ) const
{
	CAnimatedCoreModel * l_Core =  m_pCurrentAnimatedModel->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( _AnimationName );
	return i;
}