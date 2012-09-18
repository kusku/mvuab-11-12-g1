#include "RabbitDeathState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "SoundManager.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitIdleState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitDeathAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDeathState::CRabbitDeathState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CRabbitDeathState")
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_DEATH_STATE);
}

CRabbitDeathState::CRabbitDeathState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_DEATH_STATE);
}


CRabbitDeathState::~CRabbitDeathState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDeathState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = "Enter death";
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
	}
#endif

	m_pAnimationCallback->Init();
	PlayRandomSound();
}

void CRabbitDeathState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			// Volvemos al estado anterior
			/*m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());*/
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "Mort enemic";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
				}
			#endif
			m_pRabbit->SetEnable(false);	
			return;

		}
		// En otro caso actualizamos el tiempo de animacion sin hacer nada realmente pq el callback ya lo hace
		else
		{
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "DEATH NOT FINISHED YET!";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
				}
			#endif
		}
	}
	else
	{
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetDeathAnimationState());
		m_pAnimationCallback->StartAnimation();

		//m_pRabbit->FaceTo( m_pRabbit->GetSteeringEntity()->GetPosition(), _ElapsedTime );
		//m_pRabbit->MoveTo2( Vect3f(0,0,0), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				std::string l_State = "Death Animacion started";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
			}
		#endif
	}
}


void CRabbitDeathState::OnExit( CCharacter* _pCharacter )
{
}

bool CRabbitDeathState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	return false;
}


// Devuelve el tiempo, la duración
void CRabbitDeathState::PlayRandomSound( void )
{
	int l_Num = BoostRandomHelper::GetInt(1,4);
	if ( l_Num == 1 )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitsDeath1");
		m_SoundDuration = 1.685f;
	}
	else if ( l_Num == 2)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitsDeath2");
		m_SoundDuration = 1.550f;
	}
	else if ( l_Num == 3)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitsDeath3");
		m_SoundDuration = 2.252f;
	}
	else if ( l_Num == 4)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitsDeath4");
		m_SoundDuration = 1.805f;
	}
	else if ( l_Num == 5)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitsDeath5");
		m_SoundDuration = 1.742f;
	}
	else if ( l_Num == 6)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitsDeath6");
		m_SoundDuration = 1.952f;
	}
}