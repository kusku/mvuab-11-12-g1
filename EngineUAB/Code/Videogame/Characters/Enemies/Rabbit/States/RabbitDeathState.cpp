#include "RabbitDeathState.h"
#include "GameProcess.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitIdleState.h"

#include "RabbitDeathAnimationState.h"
#include "RabbitIdleAnimationState.h"

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
CRabbitDeathState::CRabbitDeathState( void )
	: CState				("CRabbitDeathState")
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEATH_STATE);
}

CRabbitDeathState::CRabbitDeathState( const std::string &_Name )
	: CState				(_Name)
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEATH_STATE);
}


CRabbitDeathState::~CRabbitDeathState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDeathState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Enter death");
	}
#endif

	m_pAnimationCallback->Init();
}

void CRabbitDeathState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
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
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Mort enemic");
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
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("DEATH NOT FINISHED YET!");
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
				CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Death Animacion started");
			}
		#endif
	}
}


void CRabbitDeathState::OnExit( CCharacter* _Character )
{
	// nos volvemos
	/*m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
	m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());*/
}

bool CRabbitDeathState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	return false;
}

