
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
#include "Math\Vector2.h"

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
	m_pAnimationsStates = NULL;
	
	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pIdleState );

	CHECKED_DELETE ( m_pAnimationIdleState );
	CHECKED_DELETE ( m_pAnimationPursuitState );
}

void CEnemy::Update( float _ElapsedTime ) //, CCamera *_pCamera)
{
	if(m_pController == NULL || m_pCurrentAnimatedModel == NULL  || _ElapsedTime == 0)
	{
		return;
	}

	Vect3f	l_PosAnterior = m_pController->GetPosition();
	UpdateMovement( _ElapsedTime );
	Vect3f	l_PosActual	= m_pController->GetPosition();

	if ( l_PosAnterior != l_PosActual )
	{
		m_pLogicStateMachine->ChangeState	 ( m_pPursuitState);
		m_pGraphicStateMachine->ChangeState  ( m_pAnimationPursuitState );
	}
	else
	{
		m_pLogicStateMachine->ChangeState	( m_pIdleState );
		m_pGraphicStateMachine->ChangeState ( m_pAnimationIdleState );
	}
}

void CEnemy::UpdateMovement(float _ElapsedTime)
{

	if(m_CurrentWPIndex == m_WavePoints.size())
	{
		GetWavePoint();

		if(m_WavePoints.size() == 0)
		{
			return;
		}
	}

	Vect2f pointA(m_WavePoints[m_CurrentWPIndex].x, m_WavePoints[m_CurrentWPIndex].z);
	Vect2f pointB(m_Position.x, m_Position.z);

	if(pointA.SqDistance(pointB) < 0.3f)
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

	Vect3f pointA2(m_WavePoints[m_CurrentWPIndex].x, 0, m_WavePoints[m_CurrentWPIndex].z);
	Vect3f pointB2(m_Position.x, 0, m_Position.z);
	Vect3f l_Position = Vect3f(0.0f, 0.0f, 0.0f);
	Vect3f l_Dir = (pointA2 - pointB2).Normalize();

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

void CEnemy::Render( CRenderManager *_RM )
{
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