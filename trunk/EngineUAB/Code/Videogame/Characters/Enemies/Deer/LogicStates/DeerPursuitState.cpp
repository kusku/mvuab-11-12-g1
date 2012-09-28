#include "DeerPursuitState.h"
#include "DeerIdleState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerHitState.h"
#include "Utils\BoostRandomHelper.h"

#include "SoundManager.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "Characters\StatesDefs.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Pursuit.h"
#include "Steering Behaviors\Seek.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Particles\ParticleEmitter.h"
#include "Particles\ParticleEmitterManager.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerPursuitState::CDeerPursuitState( CCharacter* _pCharacter )
	: CState						( _pCharacter, "CDeerPursuitState" )
	, m_pDeer						( NULL )
	, m_SoundActionStateCallback	( 0, 3 )
	, m_RunActionStateCallback		( 0, 3 )
	, m_FirtsStepDone				( false )
	, m_SecondStepDone				( false )
{
}

CDeerPursuitState::CDeerPursuitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState						( _pCharacter, _Name )
	, m_pDeer						( NULL )
	, m_SoundActionStateCallback	( 0, 3 )
	, m_RunActionStateCallback		( 0, 3 )
	, m_FirtsStepDone				( false )
	, m_SecondStepDone				( false )
{
}

CDeerPursuitState::~CDeerPursuitState( void )
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

void CDeerPursuitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_pDeer->GetBehaviors()->GetPursuit()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
	m_pDeer->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pDeer->GetPlayer()->GetSteeringEntity() );
	m_pDeer->GetBehaviors()->PursuitOn();
		
	m_pDeer->GetBehaviors()->SeparationOn();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOn();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOn();

	// Esta clase la uso como gestor de sonido para ver cuando debo enviar otro sonido
	float m_SoundDuration = BoostRandomHelper::GetFloat(1.8f, 2.f);
	m_SoundActionStateCallback.InitAction(0, m_SoundDuration);
	m_SoundActionStateCallback.StartAction();

	m_RunActionStateCallback.InitAction(0, m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_RUN_STATE) );
	m_RunActionStateCallback.StartAction();

	 m_FirtsStepDone	= false;
	 m_SecondStepDone	= false;
}

void CDeerPursuitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->PursuitOff();

	if ( m_pDeer->IsEnemyPreparedToAttack() ) 
	{
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetPreparedToAttack());
		return;
	}

	if ( m_pDeer->IsPlayerChased() ) 
	{
		// Seguimos persiguiendo...
		m_pDeer->GetBehaviors()->GetPursuit()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
		m_pDeer->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pDeer->GetPlayer()->GetSteeringEntity() );
		m_pDeer->GetBehaviors()->PursuitOn();

		m_pDeer->FaceTo(m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
		m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

		UpdateParticles(m_pDeer);
		UpdateActions(m_pDeer, _ElapsedTime);
		UpdateSounds(m_pDeer, _ElapsedTime);

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				std::string l_State = "Pursuing";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
				//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s pursuit...", m_pRabbit->GetName().c_str() );
			}
		#endif
	}
	else
	{
		// Volvemos al reposo
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetIdleState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	}
}

void CDeerPursuitState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->PursuitOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();

	m_pDeer->GetBehaviors()->SeparationOff();
	m_pDeer->GetBehaviors()->CohesionOff();
	//m_pDeer->GetBehaviors()->AlignmentOff();

	CORE->GetSoundManager()->PlayEvent("Stop_EFX_WolfRun");

	StopParticles(_pCharacter);
}

bool CDeerPursuitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
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

void CDeerPursuitState::UpdateParticles( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, "DeerStepLeft", _pCharacter->GetName() + "_StepLeft", "Bip001 R Foot");
	SetParticlePosition(_pCharacter, "DeerStepRight", _pCharacter->GetName() + "_StepRight","Bip001 L Foot");
}

void CDeerPursuitState::StopParticles( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfStepLeft", _pCharacter->GetName() + "_StepLeft")->StopEjectParticles();
	GetParticleEmitterInstance("WolfStepRight", _pCharacter->GetName() + "_StepRight")->StopEjectParticles();
}


void CDeerPursuitState::UpdateActions(CCharacter* _pCharacter, float _ElapsedTime )
{
	m_RunActionStateCallback.Update(_ElapsedTime);

	if ( m_RunActionStateCallback.IsActionFinished() )
	{
		m_RunActionStateCallback.InitAction();
		m_RunActionStateCallback.StartAction();

		m_FirtsStepDone		= false;
		m_SecondStepDone	= false;
	}
			
	if ( m_RunActionStateCallback.IsActionInTime( 0.3f ) && !m_FirtsStepDone )
	{
		GetParticleEmitterInstance("DeerStepLeft", _pCharacter->GetName() + "_StepLeft")->EjectParticles();
		m_FirtsStepDone	= true;
	}

	if ( m_RunActionStateCallback.IsActionInTime( 0.5f ) && !m_SecondStepDone )
	{
		GetParticleEmitterInstance("DeerStepRight", _pCharacter->GetName() + "_StepRight")->EjectParticles();
		m_SecondStepDone = true;
	}
}
			
void CDeerPursuitState::UpdateSounds( CCharacter* _pCharacter, float _ElapsedTime  )
{
	m_SoundActionStateCallback.Update(_ElapsedTime);
	
	if ( m_SoundActionStateCallback.IsActionStarted() )
	{
		// Miramos si ya está lanzado el sonido
		if ( !m_AlreadyChased && m_pDeer->IsPlayerDetected() ) 
		{
			//if ( m_RunWarningSounds )
				CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Deer_Run" );
			/*else
				CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_warning" );*/

			m_AlreadyChased = true;
		}

		// Si finalizó el tiempo --> finaliza el sonido
		if ( m_SoundActionStateCallback.IsActionFinished() )
		{
			m_SoundActionStateCallback.InitAction();
		}
		else
		{
			// Actualizo el gestor de sonido de este estado
			m_SoundActionStateCallback.Update(_ElapsedTime);
		}
	}
	else
	{
		m_SoundActionStateCallback.StartAction();
		m_AlreadyChased = false;
		m_RunWarningSounds = !m_RunWarningSounds;
	}
}