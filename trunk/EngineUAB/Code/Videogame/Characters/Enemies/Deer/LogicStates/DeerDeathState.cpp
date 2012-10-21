#include "DeerDeathState.h"
#include "GameProcess.h"
#include "SoundManager.h"

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
CDeerDeathState::CDeerDeathState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CDeerDeathState")
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),DEER_DEATH_STATE);
}

CDeerDeathState::CDeerDeathState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),DEER_DEATH_STATE);
}


CDeerDeathState::~CDeerDeathState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerDeathState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = DEER_DEATH_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
	}
#endif

	// --- Para la gestión del retroceso ---
	m_InitialHitPoint = m_pDeer->GetPosition();
	CProperties * l_Properties = m_pDeer->GetProperties();
	float l_MaxHitSpeed = l_Properties->GetHitRecoilSpeed()/2;
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(l_MaxHitSpeed);
	// Me dice la distancia máxima y posición que recorro cuando pega el player y bloqueo hacia atras
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance()/2;
		
	m_HitDirection = m_pDeer->GetSteeringEntity()->GetFront();
	m_HitDirection.Normalize();
	m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
	m_HitDirection = m_HitDirection * l_MaxHitSpeed;
	// ---------------------------------------
	
	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Deer_Die");
}

void CDeerDeathState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
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
					std::string l_State = "Mort enemic";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
					CORE->GetDebugGUIManager()->GetDebugRender()->DeleteEnemyStateName(m_pDeer->GetName().c_str());
				}
			#endif
			m_pAnimationCallback->Init();
			m_pDeer->SetEnable(false);
			return;

		}
		// En otro caso actualizamos el tiempo de animacion sin hacer nada realmente pq el callback ya lo hace
		else
		{
			float l_Distance = m_pDeer->GetPosition().Distance(m_InitialHitPoint);
			if ( l_Distance >= m_MaxHitDistance ) 
			{
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				return;
			} 
			else
			{
				m_pDeer->MoveTo2(m_HitDirection, _ElapsedTime );
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "DEATH NOT FINISHED YET!";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
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
				std::string l_State = "Death Animacion started";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
			}
		#endif
	}
}


void CDeerDeathState::OnExit( CCharacter* _pCharacter )
{
	m_pDeer->SetEnable(false);
	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerDieingCurta"); 
}

bool CDeerDeathState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	return false;
}

