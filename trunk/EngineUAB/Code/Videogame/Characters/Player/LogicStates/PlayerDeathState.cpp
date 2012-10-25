#include "PlayerDeathState.h"
#include "GameProcess.h"
#include "SoundManager.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Player\Player.h"

#include "PlayerIdleState.h"

#include "Characters\Player\AnimationStates\PlayerAnimationDeathState.h"

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
CPlayerDeathState::CPlayerDeathState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CPlayerDeathState")
	, m_pPlayer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), "die");
}

CPlayerDeathState::CPlayerDeathState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pPlayer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), "die");
}


CPlayerDeathState::~CPlayerDeathState(void)
{
	m_pPlayer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CPlayerDeathState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pPlayer) 
	{
		m_pPlayer = dynamic_cast<CPlayer*> (_pCharacter);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = PLAYER_DEATH_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pPlayer->GetName().c_str(), l_State );
	}
#endif

	// --- Para la gestión del retroceso ---
	m_InitialHitPoint = m_pPlayer->GetPosition();
	CProperties * l_Properties = m_pPlayer->GetProperties();
	float l_MaxHitSpeed = l_Properties->GetHitRecoilSpeed()/2;
	m_pPlayer->GetSteeringEntity()->SetMaxSpeed(l_MaxHitSpeed);
	// Me dice la distancia máxima y posición que recorro cuando pega el player y bloqueo hacia atras
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance()/2;
		
	m_HitDirection = m_pPlayer->GetSteeringEntity()->GetFront();
	m_HitDirection.Normalize();
	m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
	m_HitDirection = m_HitDirection * l_MaxHitSpeed;
	// ---------------------------------------
	
	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_CapeDeath1" );

	// Gestión de partículas. Metemos sangre!!
	//UpdateImpact(_pCharacter);
	//GenerateImpact(_pCharacter);
}

void CPlayerDeathState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pPlayer) 
	{
		m_pPlayer = dynamic_cast<CPlayer*> (_pCharacter);
	}
	
	// Actualizamos la posición
//	UpdateImpact(_pCharacter);

	// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "Mort enemic";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pPlayer->GetName().c_str(), l_State );
					CORE->GetDebugGUIManager()->GetDebugRender()->DeleteEnemyStateName(m_pPlayer->GetName().c_str());
				}
			#endif
			m_pAnimationCallback->Init();
			m_pPlayer->SetEnable(false);
			SCRIPT->RunCode("change_to_game_over_gui_process()");
			return;

		}
		// En otro caso actualizamos el tiempo de animacion sin hacer nada realmente pq el callback ya lo hace
		else
		{
			float l_Distance = m_pPlayer->GetPosition().Distance(m_InitialHitPoint);
			if ( l_Distance >= m_MaxHitDistance ) 
			{
				m_pPlayer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pPlayer->MoveTo2( m_pPlayer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				return;
			} 
			else
			{
				m_pPlayer->MoveTo2(m_HitDirection, _ElapsedTime );
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "DEATH NOT FINISHED YET!";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pPlayer->GetName().c_str(), l_State );
				}
			#endif
		}
	}
	else
	{
		m_pPlayer->GetGraphicFSM()->ChangeState(m_pPlayer->GetAnimationState(PLAYER_DEATH_ANIMATION_STATE));
		m_pAnimationCallback->StartAnimation();
		  
		//m_pPlayer->FaceTo( m_pPlayer->GetSteeringEntity()->GetPosition(), _ElapsedTime );
		//m_pPlayer->MoveTo2( Vect3f(0,0,0), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				std::string l_State = "Death Animacion started";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pPlayer->GetName().c_str(), l_State );
			}
		#endif
	}
}


void CPlayerDeathState::OnExit( CCharacter* _pCharacter )
{
	m_pPlayer->SetEnable(false);
	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_PlayerDieingCurta"); 
}

bool CPlayerDeathState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	return false;
}


void CPlayerDeathState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("PlayerBloodSplash", _pCharacter->GetName() + "_PlayerBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("PlayerBloodDust",	  _pCharacter->GetName() + "_PlayerBloodDust")->EjectParticles();
	GetParticleEmitterInstance("PlayerBlood",		  _pCharacter->GetName() + "_PlayerBlood")->EjectParticles();

	// Los impactos que realiza la cape!!
	//GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->EjectParticles();
}

void CPlayerDeathState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition(); 
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "PlayerBloodSplash", _pCharacter->GetName() + "_PlayerBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "PlayerBloodDust",	_pCharacter->GetName() + "_PlayerBloodDust",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "PlayerBlood",		_pCharacter->GetName() + "_PlayerBlood",	"", l_Pos);

	// Los impactos que realiza la cape!!
	//SetParticlePosition(_pCharacter, "CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1", "", l_Pos);
}

void CPlayerDeathState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("PlayerBloodSplash", _pCharacter->GetName() + "_PlayerBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("PlayerBloodDust",	  _pCharacter->GetName() + "_PlayerBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("PlayerBlood",		  _pCharacter->GetName() + "_PlayerBlood")->StopEjectParticles();

	// Los impactos que realiza la cape!!
	//GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->StopEjectParticles();
}