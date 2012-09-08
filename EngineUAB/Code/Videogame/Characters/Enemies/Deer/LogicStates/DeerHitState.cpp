#include "DeerHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Characters\Enemies\Deer\Deer.h"
#include "Characters\StatesDefs.h"

#include "DeerPursuitState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerTiredState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeeridleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerHitState::CDeerHitState( void )
	: CState				("CDeerHitState")
	, m_pDeer				( NULL )
	, m_pActionState		( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(HIT_STATE);
}

CDeerHitState::CDeerHitState( const std::string &_Name )
	: CState				(_Name)
	, m_pDeer				( NULL )
	, m_pActionState		( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(HIT_STATE);
}


CDeerHitState::~CDeerHitState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
	CHECKED_DELETE ( m_pActionState );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerHitState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	m_pAnimationCallback->Init();
	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
	m_pAnimationCallback->StartAnimation();
}

void CDeerHitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{*/
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pDeer->GetLogicFSM()->RevertToPreviousState();
			//m_pDeer->GetGraphicFSM()->RevertToPreviousState();

			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
		}
	/*}
	else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	/*if ( m_pActionState->IsActionFinished() )
	{
		m_pDeer->GetLogicFSM()->RevertToPreviousState();
		m_pDeer->GetGraphicFSM()->RevertToPreviousState();
	}
	else
	{
		m_pActionState->Update(_ElapsedTime);
	}*/
}

void CDeerHitState::OnExit( CCharacter* _Character )
{
}

bool CDeerHitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	//if ( _Telegram.Msg == Msg_Attack ) 
	//{
	//	/*m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
	//	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());*/
	//	return true;
	//}

	return false;
}


