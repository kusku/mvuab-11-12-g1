#include "RabbitIdleState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitHitState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdle2AnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitRunAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitIdleState::CRabbitIdleState( void )
	: CState		("CRabbitIdleState")
	, m_ActionTime  ( CActionStateCallback( 1.f, 2.f ) )
	, m_pRabbit		( NULL )
{
}

CRabbitIdleState::CRabbitIdleState( const std::string &_Name )
	: CState		(_Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pRabbit		( NULL )
{
}


CRabbitIdleState::~CRabbitIdleState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitIdleState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	m_ActionTime.InitAction();
	m_ActionTime.StartAction();
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = RABBIT_IDLE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif
}

void CRabbitIdleState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	if ( m_pRabbit->IsPlayerDetected() ) 
	{
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetPursuitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
	}
	else
	{
		if ( m_ActionTime.IsActionFinished() )
		{
			// pillamos la animación idle que deseemos
			if ( BoostRandomHelper::GetInt(1, 4) == 1 ) 
			{
				//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pRabbit->GetIdleAnimationState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = RABBIT_IDLE_STATE;
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
					}
				#endif
			}
			else
			{
				//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pRabbit->GetIdle2AnimationState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdle2AnimationState());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = RABBIT_IDLE2_STATE;
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
					}
				#endif
			}

			// pillamos un tiempo de ejecución aleatorio
			float l_TotalTime = BoostRandomHelper::GetFloat(1, 4);
			m_ActionTime.InitAction(0, l_TotalTime );
			m_ActionTime.StartAction();
		}
		else
		{
			m_ActionTime.Update(_ElapsedTime);
		}
	}

	// Reseteamos la velocidad del enemigo
	m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pRabbit->MoveTo2(_Character->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CRabbitIdleState::OnExit( CCharacter* _Character )
{
}

bool CRabbitIdleState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
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