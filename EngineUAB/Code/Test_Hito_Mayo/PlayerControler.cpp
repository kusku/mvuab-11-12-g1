#include "PlayerControler.h"
#include "RenderManager.h"

#include "PlayerDef.h"

#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicSphericalJoint.h"
#include "PhysicController.h"
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
	//CHECKED_DELETE ( m_pPhysicUserDataCub );
	//CHECKED_DELETE ( m_pBoxActor );
}

void CPlayerControler::Update ( float _ElapsedTime )
{
	UpdateInputActions(_ElapsedTime);
}
	
void CPlayerControler::Render ( CRenderManager * _RM )
{
}
	
// -----------------------------------------
//				  MÈTODES 
// -----------------------------------------
void CPlayerControler::UpdateInputActions ( float _ElapsedTime )
{
	Vect3f l_Direccio = Vect3f( 0.f, 0.f, 0.f );
	float l_Yaw			= m_pController->GetYaw();
	float l_Pitch		= m_pController->GetPitch();
	Vect3f l_Position	= m_pController->GetPosition();

	l_Pitch = 0.f;
	l_Position = l_Direccio; //Vect3f( 0.f,-9.8f ,0.f);
	///gDefaultGravity(0,-9.8,0);

	CActionToInput *l_Action2Input = CORE->GetActionToInput();
	// Comprovem el moviment del player	
	if ( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_FOWARD ) )
	{
		if ( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
		{
			l_Direccio = Vect3f( cosf( l_Yaw + D3DX_PI/4.f ), l_Pitch, sinf( l_Yaw + D3DX_PI/4.f ) );
			l_Position +=  l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
		}
		else if ( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
		{
			l_Direccio = Vect3f( cosf( l_Yaw - D3DX_PI/4.f ), l_Pitch, sinf(l_Yaw - D3DX_PI/4.f ) );
			l_Position += l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
		}
		else
		{
			l_Direccio = Vect3f ( cosf ( l_Yaw ) , l_Pitch, sinf ( l_Yaw ) );
			l_Position += l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
		}
	}
	else if ( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_BACK ) )
	{
		if ( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
		{
			l_Direccio = ( Vect3f (cosf ( l_Yaw - D3DX_PI/4) , 0, sinf ( l_Yaw - D3DX_PI/4) ) );
			l_Position -= l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
		
		}
		else if ( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
		{
			l_Direccio = ( Vect3f ( cosf ( l_Yaw + D3DX_PI/4) , 0, sinf ( l_Yaw + D3DX_PI/4 ) ) );
			l_Position -= l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
		}
		else
		{
			l_Direccio = Vect3f ( cosf ( l_Yaw ) , l_Pitch, sinf ( l_Yaw ) );
			l_Position -= l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
		}
		
	}
	else if( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
	{
		l_Direccio = Vect3f ( cosf ( l_Yaw + D3DX_PI/2) , 0, sinf ( l_Yaw + D3DX_PI/2) );
		l_Position -= l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
	}
	else if( l_Action2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
	{
		l_Direccio = Vect3f ( cosf ( l_Yaw + D3DX_PI/2) , 0, sinf ( l_Yaw + D3DX_PI/2) );
		l_Position += l_Direccio * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
	}

	UpdateMovementControler ( _ElapsedTime, l_Position );
}

void CPlayerControler::UpdateMovementControler ( float _ElapsedTime, const Vect3f &_Direccio )
{
	m_pController->Move( _Direccio, _ElapsedTime );
}

// -----------------------------------------
//				PROPIETATS
// -----------------------------------------