#include "WolfDeathState.h"
#include "GameProcess.h"
#include "EngineProcess.h"
#include "SoundManager.h"
#include "HUD/HUD.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfDeathAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

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
CWolfDeathState::CWolfDeathState( CCharacter* _pCharacter )
	: CState				(_pCharacter,"CWolfDeathState")
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_DEATH_STATE);
}

CWolfDeathState::CWolfDeathState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter,_Name)
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_DEATH_STATE);
}


CWolfDeathState::~CWolfDeathState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfDeathState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = WOLF_DEATH_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
	}
#endif

	// --- Para la gestión del retroceso ---
	m_InitialHitPoint = m_pWolf->GetPosition();
	CProperties * l_Properties = m_pWolf->GetProperties();
	float l_MaxHitSpeed = l_Properties->GetHitRecoilSpeed()/2;
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(l_MaxHitSpeed);
	// Me dice la distancia máxima y posición que recorro cuando pega el player y bloqueo hacia atras
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance()/2;
		
	m_HitDirection = m_pWolf->GetSteeringEntity()->GetFront();
	m_HitDirection.Normalize();
	m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
	m_HitDirection = m_HitDirection * l_MaxHitSpeed;
	// ---------------------------------------


	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_die");

	static_cast<CGameProcess*>(CORE->GetProcess())->GetHUD()->SetActiveWolfBar(false);

	// Gestión de partículas. Metemos sangre!!
	UpdateImpact(_pCharacter);
	GenerateImpact(_pCharacter);
}

void CWolfDeathState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	// Actualizamos la posición
	UpdateImpact(_pCharacter);

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
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					CORE->GetDebugGUIManager()->GetDebugRender()->DeleteEnemyStateName(m_pWolf->GetName().c_str());
				}
			#endif
			m_pAnimationCallback->Init();
			m_pWolf->SetEnable(false);	
			return;

		}
		// En otro caso actualizamos el tiempo de animacion sin hacer nada realmente pq el callback ya lo hace
		else
		{
			float l_Distance = m_pWolf->GetPosition().Distance(m_InitialHitPoint);
			if ( l_Distance >= m_MaxHitDistance ) 
			{
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				return;
			} 
			else
			{
				m_pWolf->MoveTo2(m_HitDirection, _ElapsedTime );
			}
			
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "DEATH NOT FINISHED YET!";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
				}
			#endif
		}
	}
	else
	{
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetDeathAnimationState());
		m_pAnimationCallback->StartAnimation();

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				std::string l_State = "Death Animacion started";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
			}
		#endif
	}
}

void CWolfDeathState::OnExit( CCharacter* _pCharacter )
{
	static_cast<CGameProcess*>(CORE->GetProcess())->GetHUD()->SetActiveWolfBar(false);
	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_Wolf_die"); 
}

bool CWolfDeathState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	return false;
}

void CWolfDeathState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfBloodSplash", _pCharacter->GetName() + "_WolfBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("WolfBloodDust",	  _pCharacter->GetName() + "_WolfBloodDust")->EjectParticles();
	GetParticleEmitterInstance("WolfBlood",		  _pCharacter->GetName() + "_WolfBlood")->EjectParticles();

	// Los impactos que realiza la cape!!
	GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->EjectParticles();
}

void CWolfDeathState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "WolfBloodSplash", _pCharacter->GetName() + "_WolfBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "WolfBloodDust",	_pCharacter->GetName() + "_WolfBloodDust",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "WolfBlood",		_pCharacter->GetName() + "_WolfBlood",	"", l_Pos);

	// Los impactos que realiza la cape!!
	SetParticlePosition(_pCharacter, "CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1", "", l_Pos);
}

void CWolfDeathState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfBloodSplash", _pCharacter->GetName() + "_WolfBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("WolfBloodDust",	  _pCharacter->GetName() + "_WolfBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("WolfBlood",		  _pCharacter->GetName() + "_WolfBlood")->StopEjectParticles();

	// Los impactos que realiza la cape!!
	GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->StopEjectParticles();
}
