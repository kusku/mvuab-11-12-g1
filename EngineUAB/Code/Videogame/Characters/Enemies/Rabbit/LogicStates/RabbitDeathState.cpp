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

	// --- Para la gesti�n del retroceso ---
	m_InitialHitPoint = m_pRabbit->GetPosition();
	CProperties * l_Properties = m_pRabbit->GetProperties();
	float l_MaxHitSpeed = l_Properties->GetHitRecoilSpeed()/2;
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(l_MaxHitSpeed);
	// Me dice la distancia m�xima y posici�n que recorro cuando pega el player y bloqueo hacia atras
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance()/2;
		
	m_HitDirection = m_pRabbit->GetSteeringEntity()->GetFront();
	m_HitDirection.Normalize();
	m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
	m_HitDirection = m_HitDirection * l_MaxHitSpeed;
	// ---------------------------------------

	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Death" );
	
	// Gesti�n de part�culas. Metemos sangre!!
	UpdateImpact(_pCharacter);
	GenerateImpact(_pCharacter);
}

void CRabbitDeathState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	// Actualizamos la posici�n
	UpdateImpact(_pCharacter);

	// Si �s atacable miro si llegu� al m�ximo de lo que permito que me golpeen y bloqueo
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animaci�n a finalizado
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
			//m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			return;

		}
		// En otro caso actualizamos el tiempo de animacion sin hacer nada realmente pq el callback ya lo hace
		else
		{
			float l_Distance = m_pRabbit->GetPosition().Distance(m_InitialHitPoint);
			if ( l_Distance >= m_MaxHitDistance ) 
			{
				m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				return;
			} 
			else
			{
				m_pRabbit->MoveTo2(m_HitDirection, _ElapsedTime );
			}

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
	m_pRabbit->SetEnable(false);
}

bool CRabbitDeathState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	return false;
}

void CRabbitDeathState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("RabbitBloodDust",	_pCharacter->GetName() + "_RabbitBloodDust")->EjectParticles();
	GetParticleEmitterInstance("RabbitBlood",		_pCharacter->GetName() + "_RabbitBlood")->EjectParticles();

	// Los impactos que realiza la cape!!
	GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->EjectParticles();
}

void CRabbitDeathState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetSteeringEntity()->GetPosition();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "RabbitBloodDust",	  _pCharacter->GetName() + "_RabbitBloodDust",	 "", l_Pos );
	SetParticlePosition(_pCharacter, "RabbitBlood",		  _pCharacter->GetName() + "_RabbitBlood",	"",		 l_Pos );
																				   
	// Los impactos que realiza la cape!!	
	SetParticlePosition(_pCharacter, "CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1", "", l_Pos );
}

void CRabbitDeathState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBloodDust",	_pCharacter->GetName() + "_RabbitBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBlood",		_pCharacter->GetName() + "_RabbitBlood")->StopEjectParticles();

	// Los impactos que realiza la cape!!										   
	GetParticleEmitterInstance("CaperucitaImpact1",		_pCharacter->GetName() + "_CaperucitaImpact1")->StopEjectParticles();
}