#include "Player.h"
#include "Characters\PlayerDef.h"

#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\FPSCamera.h"

#include "ActionToInput.h"
#include "RenderManager.h"

#include "PhysicController.h"

#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "StatesMachine\EntityManager.h"
#include "States\IdleState.h"
#include "States\PursuitState.h"

#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CPlayer::CPlayer( void )
	: CCharacter			( 0 )					// El player tiene el ID = 0 
	, m_vDirection			( 0.0f, 0.0f, 0.0f )
	, m_bMoverAutomatico	( false )
	, m_bLockCamera			( false )
{
}

CPlayer::CPlayer ( const std::string &_Name )
	: CCharacter			( 0, _Name )					// El player tiene el ID = 0 
	, m_vDirection			( 0.0f, 0.0f, 0.0f )
	, m_bMoverAutomatico	( false )
	, m_bLockCamera			( false )
{
}

CPlayer::~CPlayer( void )
{
	Done();
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
void CPlayer::Done ( void )
{
	if ( IsOK ( ) )
	{
		Release();
		m_bIsOk = false;
	}
}

bool CPlayer::Init ( void )
{
	m_bIsOk = CCharacter::Init();
	if ( m_bIsOk )
	{		
		CRenderableObjectsLayersManager *l_ROLayerManager = CORE->GetRenderableObjectsLayersManager();
		CRenderableObjectsManager *l_ROManager = l_ROLayerManager->GetResource("solid");
		CRenderableObject *l_RO = l_ROManager->GetInstance( "lobo1" );

		if ( !l_RO ) 
			l_ROManager->AddAnimatedMeshInstance( m_Name, Vect3f (0.f, 0.f, 0.f ) );
		else
			m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*> (l_RO);

		/*if ( m_pCurrentAnimatedModel )
		{
			m_pCurrentAnimatedModel->ClearCycle ( 0.3f );
			m_pCurrentAnimatedModel->BlendCycle ( 0, 0.3f );
		}*/
	}

	float l_Yaw = m_pCurrentAnimatedModel->GetYaw();
	m_pCurrentAnimatedModel->SetYaw( l_Yaw + mathUtils::Rad2Deg(m_fYaw) );

	m_pController->SetYaw(m_fYaw);

	return m_bIsOk;
}

void CPlayer::Release ( void )
{
}

void CPlayer::Update( float _ElapsedTime, CCamera *_Camera )
{
	if( !m_bLockCamera )
	{
		float d = 0.0f;
		Vect3f l_Dir = Vect3f(0.f, 0.f, 0.f);
		Vect3f l_Position = Vect3f(0.f, 0.f, 0.f);

		CActionToInput *action2Input = CORE->GetActionToInput();
		CThPSCamera* l_ThPSCamera = static_cast<CThPSCamera*>(_Camera);

		m_fYaw = m_pController->GetYaw();
		if ( action2Input->DoAction("YawViewerCam", d) )
		{
			m_fYaw += d;
			if( m_fYaw > e2PIf )
			{
				m_fYaw -= e2PIf;
			}
			else if( m_fYaw < -e2PIf )
			{
				m_fYaw += e2PIf;
			}
		}

		if( action2Input->DoAction( ACTION_MOVE_PLAYER_FOWARD ) )
		{
			if( action2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
			{
				l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), 0.f, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
				l_Position += l_Dir * MOMENTUM * _ElapsedTime;
			}
			else if( action2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
			{
				l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), 0.f, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
				l_Position += l_Dir * MOMENTUM * _ElapsedTime;
			}
			else
			{
				l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), 0.f, mathUtils::Sin<float>(m_fYaw));
				l_Position += l_Dir * MOMENTUM * _ElapsedTime;
			}
		}
		else if( action2Input->DoAction( ACTION_MOVE_PLAYER_BACK ) )
		{
			if( action2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
			{
				l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), 0.f, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
				l_Position -= l_Dir * MOMENTUM * _ElapsedTime;
			}
			else if( action2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
			{
				l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), 0.f, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
				l_Position -= l_Dir * MOMENTUM * _ElapsedTime;
			}
			else
			{
				l_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), 0.f, mathUtils::Sin<float>(m_fYaw));
				l_Position -= l_Dir * MOMENTUM * _ElapsedTime;
			}
		}

		m_pController->SetYaw(m_fYaw);
		CCharacter::MoveController(l_Position, _ElapsedTime);
		CCharacter::Update( _ElapsedTime );

		m_Position = m_pController->GetPosition();
		m_Position.y = m_Position.y - m_pController->GetHeight();
		float yaw = mathUtils::Rad2Deg(m_fYaw);
		m_pCurrentAnimatedModel->SetYaw(-yaw + 90.f );
		m_pCurrentAnimatedModel->SetPosition( m_Position );	
	}
}

//void CPlayer::MoverManualmente ( float _ElapsedTime )
//{
//	float l_Dt;
//
//	if ( CORE->GetActionToInput()->DoAction( ACTION_LOCK_FREE_CAMERA , l_Dt  ) )
//		m_bLockCamera=!m_bLockCamera;
//	
//	if(m_bLockCamera)
//		return;
//
//	Vect3f Direccio = ( 0.f, 0.f, 0.f );
//	
//	// Comprovem la Rotació del player
//	if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_FREE_CAMERA_X , l_Dt ) )
//	{
//		m_fYaw += l_Dt;
//		if ( m_fYaw > 2.0f * FLOAT_PI_VALUE )
//			m_fYaw -= 2.0f * FLOAT_PI_VALUE;
//		else if ( m_fYaw < -2.0f * FLOAT_PI_VALUE )
//			m_fYaw -= 2.0f * FLOAT_PI_VALUE;
//	}
//}

void CPlayer::Render(CRenderManager *RM)
{
}