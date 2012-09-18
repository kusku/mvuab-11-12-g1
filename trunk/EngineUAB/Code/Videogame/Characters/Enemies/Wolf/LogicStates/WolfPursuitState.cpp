#include "WolfPursuitState.h"
#include "WolfIdleState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "Characters\Enemies\Wolf\Wolf.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Pursuit.h"
#include "Steering Behaviors\Seek.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfPursuitState::CWolfPursuitState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfPursuitState")
	, m_pWolf	( NULL )
{
}

CWolfPursuitState::CWolfPursuitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_pWolf		( NULL )
{
}

CWolfPursuitState::~CWolfPursuitState( void )
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

void CWolfPursuitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
	m_pWolf->GetBehaviors()->SeekOn();
		
	m_pWolf->GetBehaviors()->GetPursuit()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
	m_pWolf->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pWolf->GetPlayer()->GetSteeringEntity() );
	m_pWolf->GetBehaviors()->PursuitOn();
		
	m_pWolf->GetBehaviors()->SeparationOn();
	m_pWolf->GetBehaviors()->CohesionOn();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();
}

void CWolfPursuitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();

	if ( m_pWolf->IsPlayerDetected() ) 
	{
		if ( m_pWolf->IsEnemyPreparedToAttack() ) 
		{
			// Reseteamos la velocidad del enemigo y cambiamos a un estado que prepara para el ataque
			m_pWolf->GetBehaviors()->PursuitOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetPreparedToAttack());
		}
		else
		{
			// Seguimos persiguiendo...
			m_pWolf->GetBehaviors()->GetPursuit()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
			m_pWolf->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pWolf->GetPlayer()->GetSteeringEntity() );
			m_pWolf->GetBehaviors()->PursuitOn();

			/*m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
			m_pWolf->GetBehaviors()->SeekOn();*/

			m_pWolf->FaceTo(m_pWolf->GetSteeringEntity()->GetPosition(), _ElapsedTime);
			m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s pursuit...", m_pRabbit->GetName().c_str() );
			}
			#endif
		}
	}
	else
	{
		// Volvemos al reposo
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetIdleState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	}
}


void CWolfPursuitState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();

	m_pWolf->GetBehaviors()->SeparationOff();
	m_pWolf->GetBehaviors()->CohesionOff();
	//_pCharacter->GetBehaviors()->AlignmentOff();
	
}

bool CWolfPursuitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(1000); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		return true;
	}
	return false;
}