#include "Character.h"
#include "CharacterWrapper.h"
#include "StatesMachine\MessageDispatcher.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Properties/Properties.h"
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
	, CNamed					( )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
	, m_bIsEnable				( true )
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
	, m_pPhysicUserDataJugador	( NULL )
	, CNamed					( _Name )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
	, m_bIsEnable				( true )
{
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
	, m_pPhysicUserDataJugador	( NULL )
	, CNamed					( _Name )
	, CObject3D					( )
	, m_PrevPosition			( Vect3f(0.f, 0.f, 0.f) )
	, m_bLocked					( false )
	, m_bIsEnable				( true )
{
	// coloco la máquina de estados
    m_pLogicStateMachine	= new CStateMachine<CCharacter>( this );
	m_pGraphicStateMachine	= new CStateMachine<CCharacter>( this );
  //  m_pController			= new CPhysicController();

	//m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*>(CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("caperucita1"));
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
bool CCharacter::Init()
{
	// Implementación en lua!

	return true;
}

bool CCharacter::Initialize ( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup )
{
	//Create a dynamic Player     
	m_pPhysicUserDataJugador = new CPhysicUserData( m_Name );
	m_pPhysicUserDataJugador->SetColor( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController( 0.5f, m_pProperties->GetHeight(), 45.f, 0.1f, 0.5f, _Grup, m_pPhysicUserDataJugador );
	m_pController->SetPosition( _InitialPosicion );
	m_pController->SetVisible( true );
	
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
	l_CanHandle = m_pLogicStateMachine->HandleMessage( _Msg );
	l_CanHandle &= m_pGraphicStateMachine->HandleMessage( _Msg );

	return l_CanHandle;
}

void CCharacter::MoveController(const Vect3f &_Dir, float _ElapsedTime)
{
	m_pController->Move( _Dir, _ElapsedTime );
}

int CCharacter::GetAnimationID(const std::string &_AnimationName)
{
	CAnimatedCoreModel * l_Core =  m_pCurrentAnimatedModel->GetAnimatedCoreModel();
	return l_Core->GetCoreModel()->getCoreAnimationId( _AnimationName );
}

void CCharacter::SetEnable(bool enable)
{
	if( !enable )
	{
		m_pCurrentAnimatedModel->SetVisible(false);
		m_pController->SetCollision(false);
	}
	else
	{
		m_pCurrentAnimatedModel->SetVisible(true);
		m_pController->SetCollision(true);
	}

	m_bIsEnable = enable;
}