#include "Player.h"
#include "PlayerDef.h"

#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\FPSCamera.h"

#include "ActionToInput.h"
#include "RenderManager.h"

#include "PlayerControler.h"

#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "StatesMachine\EntityManager.h"

#include "Properties.h"
#include "../States/AnimationsStates.h"
#include "../States/PursuitState.h"
#include "../States/IdleState.h"
#include "../States/AnimationIdleState.h"
#include "../States/AnimationPursuitState.h"

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
	: CCharacter				( 0 )					// El player tiene el ID = 0 
	, m_pPlayerProperties		( NULL )
	, m_pPlayerAnimationsStates	( NULL )
	, m_bMoverAutomatico		( false )
	, m_bLockCamera				( false )
	, m_pPursuitState			( NULL )
	, m_pIdleState				( NULL )
	, m_pAnimationPursuitState	( NULL )
	, m_pAnimationIdleState		( NULL )
{
}

CPlayer::CPlayer ( const std::string &_Name )
	: CCharacter				( 0, _Name )			// El player tiene el ID = 0 
	, m_pPlayerProperties		( NULL )
	, m_pPlayerAnimationsStates	( NULL )
	, m_bMoverAutomatico		( false )
	, m_bLockCamera				( false )
	, m_pPursuitState			( NULL )
	, m_pIdleState				( NULL )
	, m_pAnimationPursuitState	( NULL )
	, m_pAnimationIdleState		( NULL )
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
	// Aquí ya debería tener cargadas las propiedades del player
	m_bIsOk = CCharacter::Init( m_pPlayerProperties->GetName(), m_pPlayerProperties->GetPosition() );
	
	m_pPlayerProperties->SetPitch	(-D3DX_PI/6);
	m_pPlayerProperties->SetYaw		(0.0f);
	m_pPlayerProperties->SetRoll	(0.0f);

	if ( !m_bIsOk )
		return false;

	// Inicializo estados
	m_pPursuitState				= new CPursuitState();
	m_pIdleState				= new CIdleState();
	m_pAnimationIdleState		= new CAnimationIdleState();
	m_pAnimationPursuitState	= new CAnimationPursuitState();

	CRenderableObjectsLayersManager *l_ROLayerManager = CORE->GetRenderableObjectsLayersManager();
	CRenderableObjectsManager *l_ROManager = l_ROLayerManager->GetResource("solid");
	CRenderableObject *l_RO = l_ROManager->GetInstance( m_pPlayerProperties->GetAnimationInstance() );

	if ( !l_RO ) 
		l_ROManager->AddAnimatedMeshInstance( m_pPlayerProperties->GetName(), Vect3f (0.f, 0.f, 0.f ) );
	else
		m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*> (l_RO);

	if ( m_pCurrentAnimatedModel )
	{
		// coloco el primer estado
		m_pLogicStateMachine->SetCurrentState( m_pIdleState );
		m_pGraphicStateMachine->SetCurrentState( m_pAnimationIdleState );
	}
	
	// Actualizamos el Yaw y lo asignamos al controler
	float l_Yaw = m_pCurrentAnimatedModel->GetYaw();
	m_pCurrentAnimatedModel->SetYaw( l_Yaw + mathUtils::Rad2Deg( m_pPlayerProperties->GetYaw() ) );
	m_pController->SetYaw( m_pPlayerProperties->GetYaw() );
	
	return m_bIsOk;
}

void CPlayer::Release ( void )
{
	m_pPlayerProperties = NULL;
	m_pPlayerAnimationsStates = NULL;

	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pIdleState );

	CHECKED_DELETE ( m_pAnimationIdleState );
	CHECKED_DELETE ( m_pAnimationPursuitState );

	/*CHECKED_DELETE( m_pPlayerProperties );
	CHECKED_DELETE( m_pPlayerAnimationsStates );*/
}

void CPlayer::Update( float _ElapsedTime ) //, CCamera *_pCamera)
{
	/*if (m_bMoverAutomatico)
		MoverAutomaticamente( _ElapsedTime );
	else
		MoverManualmente ( _ElapsedTime );*/

	UpdateInputActions( _ElapsedTime );
	
	// Actualizamos los estados y características generales además de mover el Físic Controler
	//Vect3f	l_PosAnterior = m_pController->GetController()->GetPosition() ;
	//CCharacter::Update( _ElapsedTime );
	//Vect3f	l_PosActual	= m_pController->GetController()->GetPosition();
	//
	//if ( l_PosAnterior != l_PosActual )
	//{
	//	l_PosActual.y -= 1.5f;
	//	m_pCurrentAnimatedModel->SetPosition ( l_PosActual );
	//	m_pLogicStateMachine->ChangeState	 ( m_pPursuitState);
	//	m_pGraphicStateMachine->ChangeState  ( m_pAnimationPursuitState );


	//	//float l_x = 0.f;
	//	//float l_y = 0.f;
	//	//float l_z = 0.f;

	//	////m_pController->GetController()->GetPosition().GetAngles ( l_x, l_y, l_z );
	//	//m_pCurrentAnimatedModel->SetYaw( m_pController->GetController()->GetYaw() );
	//	//m_pPlayerProperties->SetYaw( m_pController->GetController()->GetYaw() );
	//	//m_pCurrentAnimatedModel->SetYaw(CORE->GetCamera()->GetLookAt().GetAngleZ());
	//}
	//else
	//{
	//	m_pLogicStateMachine->ChangeState	( m_pIdleState );
	//	m_pGraphicStateMachine->ChangeState ( m_pAnimationIdleState );
	//}

	//Mueve la cámara
	//m_pPlayerProperties->SetPosition( m_pController->GetController()->GetPosition() );
	/*m_pPlayerProperties->SetYaw( m_pController->GetController()->GetYaw() );
	m_pPlayerProperties->SetPitch( m_pController->GetController()->GetPitch() );*/
	 
	//m_pPlayerProperties->SetPosition( l_Position );
	/*m_pPlayerProperties->SetYaw ( l_fYaw );
	m_pPlayerProperties->SetYaw ( l_fPitch );*/

}

bool CPlayer::HandleMessage( const Telegram& _Msg, bool _Logic, bool _Graphic )
{
	return CCharacter::HandleMessage( _Msg, _Logic, _Graphic );
}

void CPlayer::UpdateInputActions ( float _ElapsedTime )
{
	float  l_Dt		   = 0.0f;
	Vect3f l_Direction = Vect3f(0.f, 0.f, 0.f);
	Vect3f l_Position  = Vect3f(0.f, 0.f, 0.f);

	CActionToInput *l_pAction2Input =  CORE->GetActionToInput();
	
	if ( l_pAction2Input->DoAction( ACTION_LOCK_FREE_CAMERA , l_Dt  ) )
		m_bLockCamera = !m_bLockCamera;
	
	if ( m_bLockCamera )
		return;

	if( l_pAction2Input->DoAction( ACTION_PLAYER_RESPAWN ) )
	{
		m_pPlayerProperties->SetPosition( m_pPlayerProperties->GetRespawnPosition() );
		m_pController->SetPosition( m_pPlayerProperties->GetRespawnPosition() );
		m_pCurrentAnimatedModel->SetPosition( m_pPlayerProperties->GetRespawnPosition() );
		CCharacter::Update( _ElapsedTime );	// Esto moverá la parte física y actualizará estados
		return;
	}

	m_pPlayerProperties->SetYaw (m_pController->GetYaw());
	float l_fYaw = m_pPlayerProperties->GetYaw();

	if ( l_pAction2Input->DoAction("YawViewerCam", l_Dt) )
	{
		l_fYaw += l_Dt;
		if( l_fYaw > e2PIf )
		{
			l_fYaw -= e2PIf;
		}
		else if( l_fYaw < -e2PIf )
		{
			l_fYaw += e2PIf;
		}
	}

	if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_FOWARD ) )
	{
		if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
		{
			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw + ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw + ePIf/4.f));
			l_Position += l_Direction * MOMENTUM * _ElapsedTime;
		}
		else if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
		{
			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw - ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw - ePIf/4.f));
			l_Position += l_Direction * MOMENTUM * _ElapsedTime;
		}
		else
		{
			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw));
			l_Position += l_Direction * MOMENTUM * _ElapsedTime;
		}
	}
	else if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_BACK ) )
	{
		if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
		{
			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw - ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw - ePIf/4.f));
			l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
		}
		else if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
		{
			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw + ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw + ePIf/4.f));
			l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
		}
		else
		{
			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw));
			l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
		}
	}
	/*else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
	{
		l_Direction = Vect3f ( mathUtils::Cos<float>( l_fYaw + D3DX_PI/2) , 0, mathUtils::Sin<float>( l_fYaw + D3DX_PI/2) );
		l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
	}
	else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
	{
		l_Direction = Vect3f ( mathUtils::Cos<float>( l_fYaw + D3DX_PI/2) , 0, mathUtils::Sin<float>( l_fYaw + D3DX_PI/2) );
		l_Position += l_Direction * MOMENTUM * _ElapsedTime;
	}*/
	
	m_pPlayerProperties->SetYaw( l_fYaw ) ;

	m_pController->SetYaw(l_fYaw);
	m_pController->MoveController( _ElapsedTime, l_Position );
	CCharacter::Update( _ElapsedTime );

	l_Position = m_pController->GetPosition();

	l_fYaw = mathUtils::Rad2Deg(l_fYaw);
	m_pCurrentAnimatedModel->SetYaw(-l_fYaw + 90.f );
	m_pCurrentAnimatedModel->SetPosition( l_Position );	

	//CThPSCamera* l_ThPSCamera = static_cast<CThPSCamera*>(CORE->GetCamera());
	

}

//void CPlayer::UpdateInputActions ( float _ElapsedTime )
//{
//	float	l_Dt		= 0.0f;
//	float	l_fYaw		= m_pPlayerProperties->GetYaw();		
//	float	l_fPitch	= m_pPlayerProperties->GetPitch();
//	//float	l_fRoll		= m_pPlayerProperties->GetRoll();
//	Vect3f	l_Position	= m_pPlayerProperties->GetPosition();
//	Vect3f  l_Direction	= Vect3f( 0.f, 0.f, 0.f );
//	
//	CActionToInput *l_pAction2Input =  CORE->GetActionToInput();
//
//	if ( l_pAction2Input->DoAction( ACTION_LOCK_FREE_CAMERA , l_Dt  ) )
//		m_bLockCamera = !m_bLockCamera;
//	
//	if ( m_bLockCamera )
//		return;
//
//	if( l_pAction2Input->DoAction( ACTION_PLAYER_RESPAWN ) )
//	{
//		m_pPlayerProperties->SetPosition( m_pPlayerProperties->GetRespawnPosition() );
//		m_pController->SetPosition( m_pPlayerProperties->GetRespawnPosition() );
//		m_pCurrentAnimatedModel->SetPosition( m_pPlayerProperties->GetRespawnPosition() );
//		CCharacter::Update( _ElapsedTime );	// Esto moverá la parte física y actualizará estados
//		return;
//	}
//
//	// Comprovem la Rotació del player
//	if ( l_pAction2Input->DoAction( ACTION_MOVE_FREE_CAMERA_X , l_Dt ) )
//	{
//		/*l_fYaw += l_Dt;
//		if ( l_fYaw > 2.0f * FLOAT_PI_VALUE )
//			l_fYaw -= 2.0f * FLOAT_PI_VALUE;
//		else if ( l_fYaw < -2.0f * FLOAT_PI_VALUE )
//			l_fYaw -= 2.0f * FLOAT_PI_VALUE;*/
//		l_fYaw += l_Dt;
//		if( l_fYaw > e2PIf )
//		{
//			l_fYaw -= e2PIf;
//		}
//		else if( l_fYaw < -e2PIf )
//		{
//			l_fYaw += e2PIf;
//		}
//	}
//
//	/*if( l_pAction2Input->DoAction( ACTION_MOVE_FREE_CAMERA_Y, l_Dt ) )
//	{
//		l_fPitch += l_Dt;
//		l_fPitch = min ( max ( -FLOAT_PI_VALUE/2.1f, l_fPitch ), FLOAT_PI_VALUE/2.1f );
//	}*/
//
//	// Comprovem el moviment del player	
//	//if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_FOWARD ) )
//	//{
//	//	if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
//	//	{
//	//		l_Direction = Vect3f( cosf( l_fYaw + D3DX_PI/4.f ), l_fPitch, sinf( l_fYaw + ePIf/4.f ) );
//	//		l_Position +=  l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	else if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
//	//	{
//	//		l_Direction = Vect3f( cosf( l_fYaw - D3DX_PI/4.f ), l_fPitch, sinf(l_fYaw - ePIf/4.f ) );
//	//		l_Position += l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	else
//	//	{
//	//		l_Direction = Vect3f ( cosf ( l_fYaw ) , l_fPitch, sinf ( l_fYaw ) );
//	//		l_Position += l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	
//	///*	if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_UP ) )
//	//	{
//	//		l_Direction = Vect3f ( 0 , 1, 0 );
//	//		l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_DOWN ) )
//	//	{
//	//		l_Direction = Vect3f ( 0, 1, 0 );
//	//		l_Position += l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}*/
//	//}
//	//else if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_BACK ) )
//	//{
//	//	if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
//	//	{
//	//		l_Direction = ( Vect3f (cosf ( l_fYaw - D3DX_PI/4) , 0, sinf ( l_fYaw - D3DX_PI/4) ) );
//	//		l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	
//	//	}
//	//	else if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
//	//	{
//	//		l_Direction = ( Vect3f ( cosf ( l_fYaw + D3DX_PI/4) , 0, sinf ( l_fYaw + D3DX_PI/4 ) ) );
//	//		l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	else
//	//	{
//	//		l_Direction = Vect3f ( cosf ( l_fYaw ) , l_fPitch, sinf ( l_fYaw ) );
//	//		l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	
//	///*	if ( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_UP ) )
//	//	{
//	//		l_Direction = Vect3f ( 0 , 1, 0 );
//	//		l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}
//	//	else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_DOWN ) )
//	//	{
//	//		l_Direction = Vect3f ( 0, 1, 0 );
//	//		l_Position += l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//	}*/
//
//	//}
//	//else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
//	//{
//	//	l_Direction = Vect3f ( cosf ( l_fYaw + D3DX_PI/2) , 0, sinf ( l_fYaw + D3DX_PI/2) );
//	//	l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//}
//	//else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
//	//{
//	//	l_Direction = Vect3f ( cosf ( l_fYaw + D3DX_PI/2) , 0, sinf ( l_fYaw + D3DX_PI/2) );
//	//	l_Position += l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//}
//	//else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_UP ) )
//	//{
//	//	l_Direction = Vect3f ( 0 , 1, 0 );
//	//	l_Position -= l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//}
//	//else if( l_pAction2Input->DoAction( ACTION_MOVE_CAMERA_DOWN ) )
//	//{
//	//	l_Direction = Vect3f ( 0, 1, 0 );
//	//	l_Position += l_Direction * QUANTITAT_MOVIMENT_PLAYER * _ElapsedTime;
//	//}
//
//	if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_FOWARD ) )
//	{
//		if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
//		{
//			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw + ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw + ePIf/4.f));
//			l_Position += l_Direction * MOMENTUM * _ElapsedTime;
//		}
//		else if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
//		{
//			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw - ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw - ePIf/4.f));
//			l_Position += l_Direction * MOMENTUM * _ElapsedTime;
//		}
//		else
//		{
//			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw));
//			l_Position += l_Direction * MOMENTUM * _ElapsedTime;
//		}
//	}
//	else if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_BACK ) )
//	{
//		if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_LEFT ) )
//		{
//			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw - ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw - ePIf/4.f));
//			l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
//		}
//		else if( l_pAction2Input->DoAction( ACTION_MOVE_PLAYER_RIGHT ) )
//		{
//			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw + ePIf/4.f), 0.f, mathUtils::Sin<float>(l_fYaw + ePIf/4.f));
//			l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
//		}
//		else
//		{
//			l_Direction = Vect3f(mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw));
//			l_Position -= l_Direction * MOMENTUM * _ElapsedTime;
//		}
//	}
//
//
//	// Actualizamos la posición de la parte física del player
//	m_pController->SetYaw		( l_fYaw );
//	m_pController->SetPosition	( l_Position );
//	//m_pController->SetPitch		 ( l_fPitch );
//	m_pController->MoveController( _ElapsedTime, l_Position );	
//	//CCharacter::Update( _ElapsedTime );	// Esto moverá la parte física
//
//	// Se almacena la posición en el player 
//	m_pPlayerProperties->SetPosition( m_pController->GetPosition() );
//	
//	// Actualizamos la posición de la parte gráfica del player
//	m_pCurrentAnimatedModel->SetYaw		( -l_fYaw + 90.f );
//	m_pCurrentAnimatedModel->SetPosition( l_Position );
//	//m_pCurrentAnimatedModel->SetPitch	( l_fPitch );
//	
//}

// Codi MARC
//void CPlayer::UpdateInputActions( float _ElapsedTime ) //, CCamera *_pCamera)
//{
//	float d = 0.0f;
//	CActionToInput *l_pAction2Input = CORE->GetActionToInput();
//	//CThPSCamera* l_ThPSCamera = static_cast<CThPSCamera*>(_pCamera);
//	if( l_pAction2Input->DoAction("YawViewerCam", d) )
//	{
//		m_fYaw += d;
//		if( m_fYaw > e2PIf )
//		{
//			m_fYaw -= e2PIf;
//		}
//		else if( m_fYaw < -e2PIf )
//		{
//			m_fYaw += e2PIf;
//		}
//	}
//
//	if( l_pAction2Input->DoAction("PitchViewerCam", d) )
//	{
//		float l_Pitch = m_fPitch + d;
//		if( abs(l_Pitch) < ePIf/2 )
//			m_fPitch = l_Pitch;
//	}
//
//	/*if( action2Input->DoAction("MoveZViewerCam", d) )
//	{
//		m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
//		if( d > 0 )
//		{
//			m_Position -= m_Dir * 50.f * _ElapsedTime;
//		}
//		else
//		{
//			m_Position += m_Dir * 50.f * _ElapsedTime;
//		}
//	}
//
//	if( action2Input->DoAction("SlowZoomViewerCam", d) )
//	{
//		float zoom = l_ThPSCamera->GetZoom() + d;
//		l_ThPSCamera->SetZoom(zoom);
//	}
//	else if( action2Input->DoAction("ZoomViewerCam", d) )
//	{
//		float zoom = l_ThPSCamera->GetZoom() + d;
//		l_ThPSCamera->SetZoom(zoom);
//	}*/
//
//
//	if( l_pAction2Input->DoAction("MovePlayerUp") )
//	{
//		if( l_pAction2Input->DoAction("MovePlayerLeft") )
//		{
//			m_vDirection = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
//			m_Position += m_vDirection * 15.f * _ElapsedTime;
//		}
//		else if( l_pAction2Input->DoAction("MovePlayerRight") )
//		{
//			m_vDirection = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
//			m_Position += m_vDirection * 15.f * _ElapsedTime;
//		}
//		else
//		{
//			m_vDirection = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
//			m_Position += m_vDirection * 15.f * _ElapsedTime;
//		}
//	}
//	else if( l_pAction2Input->DoAction("MovePlayerDown") )
//	{
//		if( l_pAction2Input->DoAction("MovePlayerLeft") )
//		{
//			m_vDirection = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
//			m_Position -= m_vDirection * 15.f * _ElapsedTime;
//		}
//		else if( l_pAction2Input->DoAction("MovePlayerRight") )
//		{
//			m_vDirection = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
//			m_Position -= m_vDirection * 15.f * _ElapsedTime;
//		}
//		else
//		{
//			m_vDirection = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
//			m_Position -= m_vDirection * 15.f * _ElapsedTime;
//		}
//	}
//	else
//	{
//		if( l_pAction2Input->DoAction("MovePlayerLeft") )
//		{
//			m_vDirection = Vect3f(cosf(m_fYaw + ePIf/2.f), 0.0f, sinf(m_fYaw + ePIf/2.f));
//			m_Position += m_vDirection * 15.f * _ElapsedTime;
//		}
//
//		if( l_pAction2Input->DoAction("MovePlayerRight") )
//		{
//			m_vDirection = Vect3f(cosf(m_fYaw + ePIf/2.f), 0.0f, sinf(m_fYaw + ePIf/2.f));
//			m_Position -= m_vDirection * 15.f * _ElapsedTime;
//		}
//	}
//}


void CPlayer::Render( CRenderManager *_RM )
{
	Mat44f mat, trans, rot, rotPitch;
	mat.SetIdentity();
	trans.SetIdentity();
	rot.SetIdentity();
	rotPitch.SetIdentity();
	trans.Translate( m_pPlayerProperties->GetPosition() );
	//rot.SetRotByAngleY(m_fYaw);
	//rotPitch.SetRotByAngleZ(m_fPitch);

	//mat = trans * rot * rotPitch;
	mat = trans;
	_RM->SetTransform(mat);

	_RM->DrawSphere(0.5f, 7, colWHITE);
}