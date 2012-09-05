#include "WolfHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "Characters\Enemies\Wolf\Wolf.h"
#include "Characters\StatesDefs.h"

#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfTiredState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdle2AnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfHitState::CWolfHitState( void )
	: CState	("CWolfHitState")
	, m_pWolf	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(HIT_STATE);
}

CWolfHitState::CWolfHitState( const std::string &_Name )
	: CState					(_Name)
	, m_pWolf					( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(HIT_STATE);
}


CWolfHitState::~CWolfHitState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfHitState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}
	
	m_pAnimationCallback->StartAnimation();
}

void CWolfHitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	if ( m_pAnimationCallback->IsAnimationFinished() ) 
	{
		m_pWolf->GetLogicFSM()->RevertToPreviousState();
		m_pWolf->GetGraphicFSM()->RevertToPreviousState();
	}
}

void CWolfHitState::OnExit( CCharacter* _Character )
{
}

bool CWolfHitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}

	return false;
}


