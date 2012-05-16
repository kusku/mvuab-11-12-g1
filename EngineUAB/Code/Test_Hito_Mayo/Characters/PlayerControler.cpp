#include "PlayerControler.h"
#include "RenderManager.h"

#include "PlayerDef.h"

#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicSphericalJoint.h"
//#include "PhysicController.h"
#include "PhysicUserData.h"

#include "InputManager.h"
#include "ActionToInput.h"

////----PhysX Includes-------------
#undef min
#undef max
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "NxActor.h"
////--------------------------------

#include "Core.h"
#include "Base.h"

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CPlayerControler::CPlayerControler( void )
{}


CPlayerControler::~CPlayerControler( void )
{
	Done ();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CPlayerControler::Done ( void )
{
	if ( IsOK() )
	{
		Release();
		m_bIsOk = false;
	}
}

bool CPlayerControler::Init ( const std::string &_Name, const Vect3f& _vGlobalPos, const Vect3f& _vlocalPos )
{
	m_bIsOk = true;
	
	//Create a dynamic Player     
	m_pPhysicUserDataJugador = new CPhysicUserData ( _Name );
	m_pPhysicUserDataJugador->SetColor ( colYELLOW );
	m_pPhysicUserDataJugador->SetPaint ( true );

	// Creo el controlador del jugador
	m_pController = new CPhysicController ( 1.f, 1.5f, 45.f, 0.1f, 0.5f, ::ECG_PERSONATGE, m_pPhysicUserDataJugador );
	m_pController->SetPosition	( _vGlobalPos );
	m_pController->SetVisible	( true );
	
	CORE->GetPhysicsManager()->AddPhysicController ( m_pController );

	return m_bIsOk;
}

void CPlayerControler::Release ( void )
{
	CHECKED_DELETE ( m_pController );
	CHECKED_DELETE ( m_pPhysicUserDataJugador );
}

void CPlayerControler::Update ( float _ElapsedTime )
{
}

void CPlayerControler::Render ( CRenderManager * _RM )
{
}
	
// -----------------------------------------
//				  MÈTODES 
// -----------------------------------------
void CPlayerControler::MoveController ( float _ElapsedTime, const Vect3f &_Posicio )
{
	m_pController->Move( _Posicio, _ElapsedTime );
}

// -----------------------------------------
//				PROPIETATS
// -----------------------------------------