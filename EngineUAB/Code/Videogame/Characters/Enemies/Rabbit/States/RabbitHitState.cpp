#include "RabbitHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "Characters\StatesDefs.h"

#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitTiredState.h"

#include "RabbitHitAnimationState.h"
#include "RabbitIdle2AnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitHitState::CRabbitHitState( void )
	: CState	("CRabbitHitState")
	, m_pRabbit	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(HIT_STATE);
}

CRabbitHitState::CRabbitHitState( const std::string &_Name )
	: CState					(_Name)
	, m_pRabbit					( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(HIT_STATE);
}


CRabbitHitState::~CRabbitHitState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
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
	
	m_pAnimationCallback->StartAnimation();
}

void CRabbitHitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	if ( m_pAnimationCallback->IsAnimationFinished() ) 
	{
		m_pRabbit->GetLogicFSM()->RevertToPreviousState();
		m_pRabbit->GetGraphicFSM()->RevertToPreviousState();
	}
}

void CRabbitHitState::OnExit( CCharacter* _Character )
{
}

bool CRabbitHitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}

	return false;
}


