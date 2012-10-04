#include "WolfIdleState.h"
#include "Utils\BoostRandomHelper.h"
#include "SoundManager.h"
#include "Utils\Timer.h"

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
#include "WolfHowlLifeState.h"
#include "WolfHowlEnemiesState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlLifeAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlEnemiesAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfIdleState::CWolfIdleState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CWolfIdleState")
	, m_pWolf				( NULL )
	, m_AlreadyDetected		( false )
	, m_AlreadyChased		( false )
	, m_ActionStateCallback	( 0, 1 )
{
}

CWolfIdleState::CWolfIdleState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_AlreadyDetected		( false )
	, m_AlreadyChased		( false )
	, m_ActionStateCallback	( 0, 1 )
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

	// Esta clase la uso como gestor de sonido para ver cuando debo enviar otro sonido
	float l_Tiempo = BoostRandomHelper::GetFloat(2.f, 4.f);
	m_ActionStateCallback.InitAction(0.f, l_Tiempo );
	m_ActionStateCallback.StartAction();
	
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

	// Si tenemos máxima vida y acaba de aparecer el lobo llamamos mis amigos.
	if ( m_pWolf->GetCanHowlForLife() && m_pWolf->TestIfCanHowlForLife(m_pWolf->GetProperties()->GetCurrentLife(), 100) )
	{
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetHowlLifeState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlLifeAnimationState());
		m_pWolf->SetCanHowlForLife(false);
		return;
	}

	if ( m_pWolf->GetCanHowlForEnemies() && m_pWolf->TestIfCanHowlForEnemies(m_pWolf->GetProperties()->GetCurrentLife(), 100) )
	{
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetHowlEnemiesState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlEnemiesAnimationState());
		m_pWolf->SetCanHowlForEnemies(false);
		return;
	}

	/*m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetHowlLifeState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlLifeAnimationState());*/
		


	// Si debo perseguir al player
	if ( m_pWolf->IsPlayerChased() ) 
	{
		/*if ( m_CanHowl && IsVidaMultiple(m_pWolf->GetProperties()->GetLife(), 100) )
		{
			m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetHowlState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlAnimationState());
			m_CanHowl = false;
		}
		else 
		{*/
			CORE->GetSoundManager()->PlayEvent( "Stop_EFX_Wolf_Idle" );
		
			m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetPursuitState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
		//}
		return;
	}

	// Gestiono sonidos si lo detecto
	if ( m_pWolf->IsPlayerDetected() )
	{
		if ( m_ActionStateCallback.IsActionStarted() )
		{
			// Miramos si ya está lanzado el sonido
			if ( !m_AlreadyDetected && m_pWolf->IsPlayerDetected() ) 
			{
				if ( m_IdleWarningSounds )
					CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Idle" );
				else
					CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_thread" );

				m_AlreadyDetected = true;
			}

			// Si finalizó el tiempo --> finaliza el sonido
			if ( m_ActionStateCallback.IsActionFinished() )
			{
				m_ActionStateCallback.InitAction();
			}
			else
			{
				// Actualizo el gestor de sonido de este estado
				m_ActionStateCallback.Update(_ElapsedTime);
			}
		}
		else
		{
			m_ActionStateCallback.StartAction();
			m_AlreadyDetected = false;
			m_IdleWarningSounds = !m_IdleWarningSounds;
		}
	}
	else
	{
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Stop_EFX_Wolf_Idle");
		m_AlreadyDetected = false;
	}

	// Reseteamos la velocidad del enemigo
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CWolfIdleState::OnExit( CCharacter* _pCharacter )
{
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Stop_EFX_Wolf_warning" );
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
