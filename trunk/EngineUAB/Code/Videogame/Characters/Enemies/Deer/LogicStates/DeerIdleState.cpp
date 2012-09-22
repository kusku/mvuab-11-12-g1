#include "DeerIdleState.h"
#include "Utils\BoostRandomHelper.h"
#include "SoundManager.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"

#include "DeerPursuitState.h"
#include "DeerHitState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerRunAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerIdleState::CDeerIdleState( CCharacter* _pCharacter )
	: CState			(_pCharacter, "CDeerIdleState")
	, m_pDeer			( NULL )
	, m_AlreadyDetected	( false )
	, m_AlreadyChased	( false )
{	  
}

CDeerIdleState::CDeerIdleState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_pDeer		( NULL )
	, m_AlreadyDetected	( false )
	, m_AlreadyChased	( false )
{
}


CDeerIdleState::~CDeerIdleState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerIdleState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	
	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	m_AlreadyDetected = false;
	m_AlreadyChased = false;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = DEER_IDLE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerIdleState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	if ( !m_AlreadyDetected && m_pDeer->IsPlayerDetected() ) 
	{
		//CORE->GetSoundManager()->PlayEvent("Play_EFX_Deer_Enemy_Detected");
		m_AlreadyDetected = true;
	}

	if ( m_pDeer->IsPlayerChased() ) 
	{
		//CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerEnemyDetected");
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetPursuitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
	}

	// Reseteamos la velocidad del enemigo
	m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pDeer->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CDeerIdleState::OnExit( CCharacter* _pCharacter )
{
}

bool CDeerIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
		}

		m_pDeer->RestLife(50); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}