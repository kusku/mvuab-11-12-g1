#include "CharacterController.h"
#include "RenderManager.h"

#include "Characters\PlayerDef.h"

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
CCharacterController::CCharacterController( void )
{}


CCharacterController::~CCharacterController( void )
{
	Done ();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CCharacterController::Done ( void )
{
	if ( IsOK() )
	{
		Release();
		m_bIsOk = false;
	}
}

void CCharacterController::Release ( void )
{
	CHECKED_DELETE ( m_pController );
	CHECKED_DELETE ( m_pPhysicUserDataJugador );
}


	
// -----------------------------------------
//				  MÈTODES 
// -----------------------------------------
void CCharacterController::UpdateMovementControler ( float _ElapsedTime, const Vect3f &_Posicio )
{
	m_pController->Move( _Posicio, _ElapsedTime );
}

// -----------------------------------------
//				PROPIETATS
// -----------------------------------------