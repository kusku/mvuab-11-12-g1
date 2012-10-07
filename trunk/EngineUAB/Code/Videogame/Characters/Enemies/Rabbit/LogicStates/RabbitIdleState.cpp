#include "RabbitIdleState.h"
#include "Utils\BoostRandomHelper.h"
#include "SoundManager.h"

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
CRabbitIdleState::CRabbitIdleState( CCharacter* _pCharacter )
	: CState						(_pCharacter, "CRabbitIdleState")
	, m_pRabbit						( NULL )
	, m_AlreadyDetected				( false )
	, m_AlreadyChased				( false )
	, m_ActionStateCallback			( 0,1 )
	, m_SoundActionStateCallback	( 0, 3 )
{
}

CRabbitIdleState::CRabbitIdleState( CCharacter* _pCharacter, const std::string &_Name )
	: CState						(_pCharacter, _Name)
	, m_pRabbit						( NULL )
	, m_AlreadyDetected				( false )
	, m_AlreadyChased				( false )
	, m_ActionStateCallback			( 0,1 )
	, m_SoundActionStateCallback	( 0, 3 )
{
}


CRabbitIdleState::~CRabbitIdleState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitIdleState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	m_AlreadyDetected	= false;
	m_AlreadyChased		= false;
	m_IdleWarningSounds = false;

	// Esta clase la uso como gestor de animaciones para ver cuando debo cambiarla
	float l_Tiempo = BoostRandomHelper::GetFloat(2.f, 4.f);
	m_ActionStateCallback.InitAction(0.f, l_Tiempo );
	m_ActionStateCallback.StartAction();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			if ( m_pRabbit->IsEnable() )
			{	
				std::string l_State = RABBIT_IDLE_STATE;
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
			}
		}
	#endif
}

void CRabbitIdleState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	// Si debo perseguir al player
	if ( m_pRabbit->IsPlayerChased() ) 
	{
		CORE->GetSoundManager()->PlayEvent( "Stop_EFX_Rabbit_Idle" );
		
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetPursuitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
		return;
	}

	// Gestiono sonidos y animacions si lo detecto
	if ( m_pRabbit->IsPlayerDetected() ) 
	{
		UpdateAnimations(_pCharacter, _ElapsedTime);
		UpdateSounds	(_pCharacter, _ElapsedTime);
	}
	else
	{
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Stop_EFX_Rabbit_Idle");
		m_AlreadyDetected = false;
	}

	// Reseteamos la velocidad del enemigo
	m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pRabbit->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CRabbitIdleState::OnExit( CCharacter* _pCharacter )
{
	if ( !_pCharacter )
		return;

	CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Stop_EFX_Rabbit_Idle");
}

bool CRabbitIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
		}

		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}

	return false;
}

void CRabbitIdleState::UpdateAnimations ( CCharacter*, float _ElapsedTime )
{
	if ( m_ActionStateCallback.IsActionFinished() )
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
		m_ActionStateCallback.InitAction(0, l_TotalTime );
		m_ActionStateCallback.StartAction();
	}
	else
	{
		m_ActionStateCallback.Update(_ElapsedTime);
	}
}

void CRabbitIdleState::UpdateSounds( CCharacter* _pCharacter, float _ElapsedTime )
{
	if ( m_SoundActionStateCallback.IsActionStarted() )
	{
		if ( m_SoundActionStateCallback.IsActionFinished() )
		{
			m_IdleWarningSounds = !m_IdleWarningSounds;
		}
		else
		{
			m_SoundActionStateCallback.Update(_ElapsedTime);
		}
	}
	else
	{
		// Esta clase la uso como gestor de sonido para ver cuando debo enviar otro sonido
		m_SoundDuration = BoostRandomHelper::GetFloat(2.6f, 3.f);
		m_SoundActionStateCallback.InitAction(0.f, m_SoundDuration );
		m_SoundActionStateCallback.StartAction();
	
		if ( m_IdleWarningSounds )
			CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Idle" );
		else
			CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Warn" );
	}
}

	