#include "WolfPursuitState.h"
#include "WolfIdleState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfHitState.h"

#include "SoundManager.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\Wolf.h"
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

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfPursuitState::CWolfPursuitState( CCharacter* _pCharacter )
	: CState						( _pCharacter, "CWolfPursuitState")
	, m_pWolf						( NULL )
	, m_SoundActionStateCallback	( 0, 3 )
	, m_RunActionStateCallback		( 0, 3 )
	, m_FirtsStepDone				( false )
	, m_SecondStepDone				( false )
{
}

CWolfPursuitState::CWolfPursuitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState						(_pCharacter, _Name)
	, m_pWolf						( NULL )
	, m_SoundActionStateCallback	( 0, 3 )
	, m_RunActionStateCallback		( 0, 3 )
	, m_FirtsStepDone				( false )
	, m_SecondStepDone				( false )
{
}

CWolfPursuitState::~CWolfPursuitState( void )
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

void CWolfPursuitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->GetPursuit()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
	m_pWolf->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pWolf->GetPlayer()->GetSteeringEntity() );
	m_pWolf->GetBehaviors()->PursuitOn();
		
	m_pWolf->GetBehaviors()->SeparationOn();
	m_pWolf->GetBehaviors()->CohesionOn();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();
		
	m_RunActionStateCallback.InitAction(0, m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_RUN_STATE));
	m_RunActionStateCallback.StartAction();

	m_SoundDuration = BoostRandomHelper::GetFloat(2.5f, 3.0f);
	m_SoundActionStateCallback.InitAction(0, m_SoundDuration);
	m_SoundActionStateCallback.StartAction();

	m_FirtsStepDone	= false;
	m_SecondStepDone	= false;
}

void CWolfPursuitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();

	// Si debo preparar el ataque
	if ( m_pWolf->IsEnemyPreparedToAttack() ) 
	{
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetPreparedToAttack());
		return;
	}

	if ( m_pWolf->IsPlayerChased() ) 
	{
		// Seguimos persiguiendo...
		m_pWolf->GetBehaviors()->GetPursuit()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
		m_pWolf->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pWolf->GetPlayer()->GetSteeringEntity() );
		m_pWolf->GetBehaviors()->PursuitOn();

		/*m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
		m_pWolf->GetBehaviors()->SeekOn();*/

		m_pWolf->FaceTo(m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
		m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

		UpdateParticles(m_pWolf);
		UpdateActions(m_pWolf, _ElapsedTime);
		UpdateSounds(m_pWolf, _ElapsedTime);
			
		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				std::string l_State = "Pursuing";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
				//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s pursuit...", m_pRabbit->GetName().c_str() );
			}
		#endif
	}
	else
	{
		// Volvemos al reposo
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetIdleState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	}
}

void CWolfPursuitState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();

	m_pWolf->GetBehaviors()->SeparationOff();
	m_pWolf->GetBehaviors()->CohesionOff();
	//_pCharacter->GetBehaviors()->AlignmentOff();

	CORE->GetSoundManager()->PlayEvent("Stop_EFX_WolfRun");

	StopParticles(_pCharacter);
}

bool CWolfPursuitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(50); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}
	return false;
}

void CWolfPursuitState::UpdateParticles( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, "WolfStepLeft", _pCharacter->GetName() + "_StepLeft", "Bip01 R Foot");
	SetParticlePosition(_pCharacter, "WolfStepRight", _pCharacter->GetName() + "_StepRight","Bip01 L Foot");
}

void CWolfPursuitState::StopParticles( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfStepLeft", _pCharacter->GetName() + "_StepLeft")->StopEjectParticles();
	GetParticleEmitterInstance("WolfStepRight", _pCharacter->GetName() + "_StepRight")->StopEjectParticles();
}

void CWolfPursuitState::UpdateActions(CCharacter* _pCharacter, float _ElapsedTime )
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
			
void CWolfPursuitState::UpdateSounds( CCharacter* _pCharacter, float _ElapsedTime  )
{
	m_SoundActionStateCallback.Update(_ElapsedTime);
	
	if ( m_SoundActionStateCallback.IsActionStarted() )
	{
		// Miramos si ya está lanzado el sonido
		if ( !m_AlreadyChased && m_pWolf->IsPlayerDetected() ) 
		{
			if ( m_RunWarningSounds )
				CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Run" );
			else
				CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_warning" );

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
