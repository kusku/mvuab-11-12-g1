#include "RabbitHitState.h"
#include "GameProcess.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\Telegram.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "Characters\StatesDefs.h"

#include "RabbitIdleState.h"
//#include "RabbitDeathState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitHitState::CRabbitHitState( void )
	: CState				("CRabbitHitState")
	, m_pRabbit				( NULL )
	, m_pActionState		( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RABBIT_HIT_STATE);

	m_pActionState = new CActionStateCallback(0,1);
}

CRabbitHitState::CRabbitHitState( const std::string &_Name )
	: CState				(_Name)
	, m_pRabbit				( NULL )
	, m_pActionState		( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RABBIT_HIT_STATE);
	
	m_pActionState = new CActionStateCallback(0,1);
}


CRabbitHitState::~CRabbitHitState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
	CHECKED_DELETE ( m_pActionState );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitHitState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}
	/*m_pAnimationCallback->Init();
	m_pAnimationCallback->StartAnimation();*/
	m_pActionState->SetTimeRange( 0.f, m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_HIT_STATE));
}

void CRabbitHitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pRabbit->GetLogicFSM()->RevertToPreviousState();
			m_pRabbit->GetGraphicFSM()->RevertToPreviousState();
		}
	}
	else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	if ( m_pActionState->IsActionFinished() )
	{
		/*m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());*/

		if ( m_pRabbit->IsAlive() ) 
		{
			m_pRabbit->GetLogicFSM()->RevertToPreviousState();
			m_pRabbit->GetGraphicFSM()->RevertToPreviousState();
		}
		//else
		/*{
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetDeathState());
		}*/
		
	}
	else
	{
		m_pActionState->Update(_ElapsedTime);
	}
}

void CRabbitHitState::OnExit( CCharacter* _Character )
{
}

bool CRabbitHitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	return false;
}


