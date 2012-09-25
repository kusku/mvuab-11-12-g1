#include "WolfIdleState.h"
#include "Utils\BoostRandomHelper.h"
#include "SoundManager.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfIdleState::CWolfIdleState( CCharacter* _pCharacter )
	: CState			(_pCharacter, "CWolfIdleState")
	, m_pWolf			( NULL )
	, m_AlreadyDetected	( false )
	, m_AlreadyChased	( false )
{
}

CWolfIdleState::CWolfIdleState( CCharacter* _pCharacter, const std::string &_Name )
	: CState			(_pCharacter, _Name)
	, m_pWolf			( NULL )
	, m_AlreadyDetected	( false )
	, m_AlreadyChased	( false )
{
}


CWolfIdleState::~CWolfIdleState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfIdleState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	m_AlreadyDetected = false;
	m_AlreadyChased = false;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = WOLF_IDLE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}

	#endif
}

void CWolfIdleState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	if ( !m_AlreadyDetected && m_pWolf->IsPlayerDetected() ) 
	{
		//CORE->GetSoundManager()->PlayEvent("Play_EFX_Wolf_Enemy_Detected");
		m_AlreadyDetected = true;
	}


	if ( m_pWolf->IsPlayerChased() ) 
	{
		//CORE->GetSoundManager()->PlayEvent("Stop_EFX_WolfEnemyDetected");
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetPursuitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
	}
	
	// Reseteamos la velocidad del enemigo
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CWolfIdleState::OnExit( CCharacter* _pCharacter )
{
}

bool CWolfIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(1000); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}
	return false;
}