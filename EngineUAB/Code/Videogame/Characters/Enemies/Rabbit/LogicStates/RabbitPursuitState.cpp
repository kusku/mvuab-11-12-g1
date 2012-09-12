#include "RabbitPursuitState.h"
#include "RabbitIdleState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitHitState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

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

#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitPursuitState::CRabbitPursuitState( void )
	: CState	("CRabbitPursuitState")
	, m_pRabbit	( NULL )
{
}

CRabbitPursuitState::CRabbitPursuitState( const std::string &_Name )
	: CState		(_Name)
	, m_pRabbit		( NULL )
{
}

CRabbitPursuitState::~CRabbitPursuitState( void )
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

void CRabbitPursuitState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	/*m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->SeekOn();*/
		
	m_pRabbit->GetBehaviors()->GetPursuit()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pRabbit->GetPlayer()->GetSteeringEntity() );
	m_pRabbit->GetBehaviors()->PursuitOn();
		
	m_pRabbit->GetBehaviors()->SeparationOn();
	m_pRabbit->GetBehaviors()->CohesionOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();
}

void CRabbitPursuitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}
	
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();

	if ( m_pRabbit->IsPlayerDetected() ) 
	{
		if ( m_pRabbit->IsEnemyPreparedToAttack() ) 
		{
			// Reseteamos la velocidad del enemigo y cambiamos a un estado que prepara para el ataque
			/*m_pRabbit->GetBehaviors()->PursuitOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetPreparedToAttack());
		}
		else
		{
			// Seguimos persiguiendo...
			m_pRabbit->GetBehaviors()->GetPursuit()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
			m_pRabbit->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pRabbit->GetPlayer()->GetSteeringEntity() );
			m_pRabbit->GetBehaviors()->PursuitOn();

			/*m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
			m_pRabbit->GetBehaviors()->SeekOn();*/

			m_pRabbit->FaceTo(m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Pursuing");
					//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s pursuit...", m_pRabbit->GetName().c_str() );
			}
			#endif
		}
	}
	else
	{
		// Volvemos al reposo
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	}
}


void CRabbitPursuitState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();

	m_pRabbit->GetBehaviors()->SeparationOff();
	m_pRabbit->GetBehaviors()->CohesionOff();
	//_Character->GetBehaviors()->AlignmentOff();
	
}

bool CRabbitPursuitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_Character);
		}

		m_pRabbit->RestLife(1000); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		return true;
	}
	return false;
}