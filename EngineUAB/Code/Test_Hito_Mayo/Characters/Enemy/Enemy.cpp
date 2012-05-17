
#define NOMINMAX

#include "Enemy.h"
#include "Characters\PlayerDef.h"

#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\FPSCamera.h"

#include "ActionToInput.h"
#include "RenderManager.h"

//#include "PlayerController.h"
#include "PhysicController.h"
#include "CharacterController.h"

#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "StatesMachine\EntityManager.h"

#include "Characters\Properties\Properties.h"

#include "Characters\States\AnimationsStates.h"
#include "Characters\States\PursuitState.h"
#include "Characters\States\IdleState.h"
#include "Characters\States\AnimationIdleState.h"
#include "Characters\States\AnimationPursuitState.h"

#include "Math\Matrix44.h"
#include "Math\Plane.h"

#include "Base.h"
#include "Core.h"

#include "Movement\WayPointManager.h"
#include "RenderManager.h"
#include "Core.h"

#include "Movement\WayPoint.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CEnemy::CEnemy( int _ID )
	: CCharacter				( _ID )								
	, m_pEnemyProperties		( NULL )
	, m_bMoverAutomatico		( false )
	, m_bLockCamera				( false )
	, m_pPursuitState			( NULL ) 
	, m_pIdleState				( NULL )
	, m_pAnimationPursuitState	( NULL )
	, m_pAnimationIdleState		( NULL )
	, m_CurrentWPIndex			(0)
	, m_DestWavePoint			(NULL)
{

}

CEnemy::CEnemy( int _ID, const std::string &_Name )
	: CCharacter				( _ID, _Name )					
	, m_pEnemyProperties		( NULL )
	, m_bMoverAutomatico		( false )
	, m_bLockCamera				( false )
	, m_pPursuitState			( NULL )
	, m_pIdleState				( NULL )
	, m_pAnimationPursuitState	( NULL )
	, m_pAnimationIdleState		( NULL )
{
}

CEnemy::~CEnemy( void )
{
	Done();
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
void CEnemy::Done ( void )
{
	if ( IsOK ( ) )
	{
		Release();
		m_bIsOk = false;
	}
}

bool CEnemy::Init ( void )
{
	// Aquí ya debería tener cargadas las propiedades del player
	m_bIsOk = CCharacter::Init( m_pEnemyProperties->GetName(), m_pEnemyProperties->GetPosition(), ::ECG_ENEMICS );
	if ( !m_bIsOk )
		return false;

	// Inicializo estados
	m_pPursuitState				= new CPursuitState();
	m_pIdleState				= new CIdleState();
	m_pAnimationIdleState		= new CAnimationIdleState();
	m_pAnimationPursuitState	= new CAnimationPursuitState();

	CRenderableObjectsLayersManager *l_ROLayerManager = CORE->GetRenderableObjectsLayersManager();
	CRenderableObjectsManager *l_ROManager = l_ROLayerManager->GetResource("solid");
	CRenderableObject *l_RO = l_ROManager->GetInstance( m_pEnemyProperties->GetAnimationInstance() );
		
	if ( !l_RO ) 
		l_ROManager->AddAnimatedMeshInstance( m_pEnemyProperties->GetName(), Vect3f (0.f, 0.f, 0.f ) );
	else
		m_pCurrentAnimatedModel = static_cast<CAnimatedInstanceModel*> (l_RO);

	if ( m_pCurrentAnimatedModel )
	{
		// coloco el primer estado
		m_pLogicStateMachine->SetCurrentState( m_pIdleState );
		m_pGraphicStateMachine->SetCurrentState( m_pAnimationIdleState );
	}
	
	return m_bIsOk;
}

void CEnemy::Release ( void )
{
	m_pEnemyProperties = NULL;
	m_pEnemyAnimationsStates = NULL;
	
	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pIdleState );

	CHECKED_DELETE ( m_pAnimationIdleState );
	CHECKED_DELETE ( m_pAnimationPursuitState );
}

void CEnemy::Update( float _ElapsedTime ) //, CCamera *_pCamera)
{
	if(m_CurrentWPIndex == m_WavePoints.size())
	{
		GetWavePoint();

		if(m_WavePoints.size() == 0)
		{
			return;
		}
	}

	if(m_WavePoints[m_CurrentWPIndex].SqDistance(m_Position) < 0.2f)
	{
		m_CurrentWPIndex++;
		return;
	}

	Vect3f v = (m_WavePoints[m_CurrentWPIndex] - m_Position);
	float back = v.Dot(GetFront());
	if(back < 0)
	{
		m_fYaw += (mathUtils::Deg2Rad(90.0f) * _ElapsedTime);
	}
	else
	{
		m_fYaw += (-mathUtils::Deg2Rad(90.0f) * _ElapsedTime);
	}

	Vect3f l_Position = Vect3f(0.0f, 0.0f, 0.0f);
	Vect3f l_Dir = (m_WavePoints[m_CurrentWPIndex] - m_Position).Normalize();

	l_Position += l_Dir * 6.0f * _ElapsedTime;

	m_pController->SetYaw(m_fYaw);
	CCharacter::MoveController(l_Position, _ElapsedTime);
	CCharacter::Update( _ElapsedTime );

	m_Position = m_pController->GetPosition();
	m_Position.y = m_Position.y - m_pController->GetHeight();
	float yaw = mathUtils::Rad2Deg(m_fYaw);
	m_pCurrentAnimatedModel->SetYaw(-yaw + 90.f );
	m_pCurrentAnimatedModel->SetPosition( m_Position );
}

void CEnemy::GetWavePoint()
{
	m_DestWavePoint = CORE->GetWayPointManager()->GetRandomWayPoint("Group 1");
	assert(m_DestWavePoint);

	m_CurrentWPIndex = 0;
	m_WavePoints = CORE->GetWayPointManager()->GetPath("Group 1", m_Position, m_DestWavePoint->GetPosition());
}

bool CEnemy::HandleMessage( const Telegram& _Msg, bool _Logic, bool _Graphic )
{
	return CCharacter::HandleMessage( _Msg, _Logic, _Graphic );
}

void CEnemy::MoverAutomaticamente ( float _ElapsedTime )
{
	//srand (static_cast <unsigned int>(time(0)));   // nº entre 0 i 32767
	//int randomNumber = rand ();	
	//int RandomDirection = (randomNumber % 8);		// obtinc un nº entre 0 i 8 - Totes les direccions
	//Vect3f Direccio = ( 0.f, 0.f, 0.f );

	//switch ( RandomDirection )
	//{
	//	case Up: 
	//		Direccio = Vect3f ( cosf ( m_Yaw ) , m_Pitch, sinf ( m_Yaw ) );
	//		m_Position += Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//		break;
	//	case UpRight:
	//		Direccio = Vect3f( cosf( m_Yaw - D3DX_PI/4.f ), m_Pitch, sinf(m_Yaw - D3DX_PI/4.f ) );
	//		m_Position += Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//		break;
	//	case UpLeft:
	//		Direccio = Vect3f( cosf( m_Yaw + D3DX_PI/4.f ), m_Pitch, sinf( m_Yaw + D3DX_PI/4.f ) );
	//		m_Position +=  Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//		break;
	//	case Back:
	//		Direccio = Vect3f ( cosf ( m_Yaw ) , m_Pitch, sinf ( m_Yaw ) );
	//		m_Position -= Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//	case BackLeft: 
	//		Direccio = ( Vect3f (cosf ( m_Yaw - D3DX_PI/4) , m_Pitch, sinf ( m_Yaw - D3DX_PI/4) ) );
	//		m_Position -= Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//	case BackRight:
	//		Direccio = ( Vect3f ( cosf ( m_Yaw + D3DX_PI/4) , m_Pitch, sinf ( m_Yaw + D3DX_PI/4 ) ) );
	//		m_Position -= Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//	case Left:
	//		Direccio = Vect3f ( cosf ( m_Yaw + D3DX_PI/2) , m_Pitch, sinf ( m_Yaw + D3DX_PI/2) );
	//		m_Position += Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//	case Right:
	//		Direccio = Vect3f ( cosf ( m_Yaw + D3DX_PI/2) , m_Pitch, sinf ( m_Yaw + D3DX_PI/2) );
	//		m_Position -= Direccio * QUANTITAT_MOVIMENT * _ElapsedTime;
	//}
}

void CEnemy::MoverManualmente ( float _ElapsedTime )
{
	//float	l_Dt;
	//float	l_fYaw		= m_pEnemyProperties->GetYaw();
	//float	l_fPitch	= m_pEnemyProperties->GetPitch();
	//float	l_fRoll		= m_pEnemyProperties->GetRoll();
	//Vect3f	l_Position	= m_pEnemyProperties->GetPosition();

	//if ( CORE->GetActionToInput()->DoAction( ACTION_LOCK_FREE_CAMERA , l_Dt  ) )
	//	m_bLockCamera=!m_bLockCamera;
	//
	//if(m_bLockCamera)
	//	return;

	//Vect3f l_Direccio = ( 0.f, 0.f, 0.f );
	//
	//// Comprovem la Rotació del player
	//if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_FREE_CAMERA_X , l_Dt ) )
	//{
	//	l_fYaw += l_Dt;
	//	if ( l_fYaw > 2.0f * FLOAT_PI_VALUE )
	//		l_fYaw -= 2.0f * FLOAT_PI_VALUE;
	//	else if ( l_fYaw < -2.0f * FLOAT_PI_VALUE )
	//		l_fYaw -= 2.0f * FLOAT_PI_VALUE;
	//}

	//if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_FREE_CAMERA_Y, l_Dt ) )
	//{
	//	l_fPitch += l_Dt;
	//	l_fPitch = min ( max ( -FLOAT_PI_VALUE/2.1f, l_fPitch ), FLOAT_PI_VALUE/2.1f );
	//}

	//// Comprovem el moviment del player	
	//if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_FOWARD ) )
	//{
	//	if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
	//	{
	//		l_Direccio = Vect3f( cosf( l_fYaw + D3DX_PI/4.f ), l_fPitch, sinf( l_fYaw + D3DX_PI/4.f ) );
	//		l_Position +=  l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	else if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
	//	{
	//		l_Direccio = Vect3f( cosf( l_fYaw - D3DX_PI/4.f ), l_fPitch, sinf(l_fYaw - D3DX_PI/4.f ) );
	//		l_Position += l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	else
	//	{
	//		l_Direccio = Vect3f ( cosf ( l_fYaw ) , l_fPitch, sinf ( l_fYaw ) );
	//		l_Position += l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	
	//	if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_UP ) )
	//	{
	//		l_Direccio = Vect3f ( 0 , 1, 0 );
	//		l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	else if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_DOWN ) )
	//	{
	//		l_Direccio = Vect3f ( 0, 1, 0 );
	//		l_Position += l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//}
	//else if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_BACK ) )
	//{
	//	if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
	//	{
	//		l_Direccio = ( Vect3f (cosf ( l_fYaw - D3DX_PI/4) , 0, sinf ( l_fYaw - D3DX_PI/4) ) );
	//		l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	
	//	}
	//	else if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
	//	{
	//		l_Direccio = ( Vect3f ( cosf ( l_fYaw + D3DX_PI/4) , 0, sinf ( l_fYaw + D3DX_PI/4 ) ) );
	//		l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	else
	//	{
	//		l_Direccio = Vect3f ( cosf ( l_fYaw ) , l_fPitch, sinf ( l_fYaw ) );
	//		l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	
	//	if ( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_UP ) )
	//	{
	//		l_Direccio = Vect3f ( 0 , 1, 0 );
	//		l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}
	//	else if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_DOWN ) )
	//	{
	//		l_Direccio = Vect3f ( 0, 1, 0 );
	//		l_Position += l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//	}

	//}
	//else if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_RIGHT ) )
	//{
	//	l_Direccio = Vect3f ( cosf ( l_fYaw + D3DX_PI/2) , 0, sinf ( l_fYaw + D3DX_PI/2) );
	//	l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//}
	//else if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_LEFT ) )
	//{
	//	l_Direccio = Vect3f ( cosf ( l_fYaw + D3DX_PI/2) , 0, sinf ( l_fYaw + D3DX_PI/2) );
	//	l_Position += l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//}
	//else if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_UP ) )
	//{
	//	l_Direccio = Vect3f ( 0 , 1, 0 );
	//	l_Position -= l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//}
	//else if( CORE->GetActionToInput()->DoAction( ACTION_MOVE_CAMERA_DOWN ) )
	//{
	//	l_Direccio = Vect3f ( 0, 1, 0 );
	//	l_Position += l_Direccio * QUANTITAT_MOVIMENT_CAMERA * _ElapsedTime;
	//}
}

// Codi MARC
//void CEnemy::UpdateInputActions( float _ElapsedTime ) //, CCamera *_pCamera)
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


void CEnemy::Render( CRenderManager *_RM )
{
//	CORE->GetRenderManager()->SetTransform(m44fIDENTITY);
//
//	Mat44f tran = m44fIDENTITY;
//
//	//CORE->GetRenderManager()->SetTransform(tran.Translate(m_Position));
//
//	Vect3f front = m_Position;
//	Vect3f front2 = m_Position + GetFront() * 2;
//
//	CORE->GetRenderManager()->DrawLine(front, front2, colRED);
}

void CEnemy::MoveCharacterToDestination ( Vect3f _Destination, float _ElapsedTime )
{
	// Pillo la ultima posición a conseguir y me encaro sobre ella. 
	Vect3f l_LastPos = _Destination;		
	Vect3f l_LastPositionNormalizated = (l_LastPos - this->GetPosition() ).Normalize();		// Normalizo el vector dirección entre ambas posiciones

	// Calculo el ángulo
	float l_vAngle = GetFront() * l_LastPositionNormalizated;
	// Si el ángulo es cerrado y está entre la posición a donde ir voy y sino roto hasta encontrarla. Valores entre -1 i 1 (sin/cos)
	if ( l_vAngle >= 0.9f ) 
	{
		Vect3f l_NewPosition			 = GetPosition( ) + GetFront( ) * _ElapsedTime * ( (float) m_pEnemyProperties->GetSpeed() );
		Vect3f l_NewPositionNormalizated = ( l_LastPos - l_NewPosition ).Normalize();
		if ( ( l_NewPositionNormalizated * l_LastPositionNormalizated ) > 0  )
		{
			SetPosition( l_NewPosition );
		}
		else {
			SetPosition( l_LastPos );
		}
	}
	else 
	{
		float l_RotationSpeed =	0.5;	//GetRotationSpeed ();
		if( isPointAtLeft( l_LastPos ) ) {
			AddYaw( -l_RotationSpeed * _ElapsedTime );
			if( !isPointAtLeft( l_LastPos ) ) 
			{
				float l_fYaw = l_LastPositionNormalizated.GetAngleY(); // .xzToAngle ();
				SetYaw ( l_fYaw );
			}
		} else {
			AddYaw( l_RotationSpeed * _ElapsedTime );
			if( isPointAtLeft( l_LastPos ) ) 
			{
				float l_fYaw = l_LastPositionNormalizated.GetAngleY(); // xzToAngle ();
				SetYaw ( l_fYaw );
			}
		}
	}
}