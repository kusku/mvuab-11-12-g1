#include "DeerDeathState.h"
#include "GameProcess.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"

#include "DeerIdleState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerDeathAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"

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
CDeerDeathState::CDeerDeathState( void )
	: CState				("CDeerDeathState")
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_DEATH_STATE);
}

CDeerDeathState::CDeerDeathState( const std::string &_Name )
	: CState				(_Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_DEATH_STATE);
}


CDeerDeathState::~CDeerDeathState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerDeathState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Enter death");
	}
#endif

	m_pAnimationCallback->Init();
}

void CDeerDeathState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			// Volvemos al estado anterior
			/*m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());*/
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Mort enemic");
				}
			#endif
			m_pAnimationCallback->Init();
			m_pDeer->SetEnable(false);	
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
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetDeathAnimationState());
		m_pAnimationCallback->StartAnimation();

		//m_pDeer->FaceTo( m_pDeer->GetSteeringEntity()->GetPosition(), _ElapsedTime );
		//m_pDeer->MoveTo2( Vect3f(0,0,0), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Death Animacion started");
			}
		#endif
	}
}


void CDeerDeathState::OnExit( CCharacter* _Character )
{
}

bool CDeerDeathState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	return false;
}

