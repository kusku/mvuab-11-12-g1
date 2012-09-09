#include "WolfHitState.h"
#include "GameProcess.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\Telegram.h"

#include "Characters\Enemies\Wolf\Wolf.h"
#include "Characters\StatesDefs.h"

#include "WolfPursuitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdle2AnimationState.h"

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
CWolfHitState::CWolfHitState( void )
	: CState	("CWolfHitState")
	, m_pWolf	( NULL )
	, m_pActionState		( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(WOLF_HIT_STATE);

	m_pActionState = new CActionStateCallback(0,1);
}

CWolfHitState::CWolfHitState( const std::string &_Name )
	: CState				(_Name)
	, m_pWolf				( NULL )
	, m_pActionState		( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(WOLF_HIT_STATE);
	
	m_pActionState = new CActionStateCallback(0,1);
}


CWolfHitState::~CWolfHitState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
	CHECKED_DELETE ( m_pActionState );
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
	
	m_pAnimationCallback->Init();
	m_pAnimationCallback->StartAnimation();
	m_pActionState->SetTimeRange( 0.f, m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_HIT_STATE));
}

void CWolfHitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pWolf->GetLogicFSM()->RevertToPreviousState();
			m_pWolf->GetGraphicFSM()->RevertToPreviousState();
		}
	}
	else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	if ( m_pActionState->IsActionFinished() )
	{
		/*m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());*/

		if ( m_pWolf->IsAlive() ) 
		{
			m_pWolf->GetLogicFSM()->RevertToPreviousState();
			m_pWolf->GetGraphicFSM()->RevertToPreviousState();
		}
		//else
		/*{
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetDeathState());
		}*/
		
	}
	else
	{
		m_pActionState->Update(_ElapsedTime);
	}
}

void CWolfHitState::OnExit( CCharacter* _Character )
{
}

bool CWolfHitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	/*if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}
*/
	return false;
}


