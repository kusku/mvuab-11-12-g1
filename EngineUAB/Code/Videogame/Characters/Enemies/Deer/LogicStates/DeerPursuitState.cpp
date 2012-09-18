#include "DeerPursuitState.h"
#include "DeerIdleState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerHitState.h"

#include "SoundManager.h"
#include "Utils\BoostRandomHelper.h"

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
CDeerPursuitState::CDeerPursuitState( void )
	: CState						("CDeerPursuitState")
	, m_pDeer						( NULL )
	, m_SoundActionStateCallback	( 0, 3 )
	, m_RunActionStateCallback		( 0, 3 )
	, m_FirtsStepDone				( false )
	, m_SecondStepDone				( false )
{
}

CDeerPursuitState::CDeerPursuitState( const std::string &_Name )
	: CState						( _Name )
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

void CDeerPursuitState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	/*m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->SeekOn();*/
		
	m_pDeer->GetBehaviors()->GetPursuit()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
	m_pDeer->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pDeer->GetPlayer()->GetSteeringEntity() );
	m_pDeer->GetBehaviors()->PursuitOn();
		
	m_pDeer->GetBehaviors()->SeparationOn();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOn();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOn();

	PlayRandomSound();
	m_SoundActionStateCallback.InitAction(0, m_SoundDuration);
	m_SoundActionStateCallback.StartAction();

	m_RunActionStateCallback.InitAction(0, m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_RUN_STATE) );
	m_RunActionStateCallback.StartAction();

	 m_FirtsStepDone	= false;
	 m_SecondStepDone	= false;
}

void CDeerPursuitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	SetParticlePosition(m_pDeer);
				
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->PursuitOff();

	m_SoundActionStateCallback.Update(_ElapsedTime);
	m_RunActionStateCallback.Update(_ElapsedTime);

	if ( m_SoundActionStateCallback.IsActionFinished() ) 
	{
		PlayRandomSound();
		m_SoundActionStateCallback.InitAction(0, m_SoundDuration);
		m_SoundActionStateCallback.StartAction();
	}

	if ( m_pDeer->IsPlayerDetected() ) 
	{
		if ( m_pDeer->IsEnemyPreparedToAttack() ) 
		{
			// Reseteamos la velocidad del enemigo y cambiamos a un estado que prepara para el ataque
			/*m_pRabbit->GetBehaviors()->PursuitOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetPreparedToAttack());
		}
		else
		{
			// Seguimos persiguiendo...
			m_pDeer->GetBehaviors()->GetPursuit()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
			m_pDeer->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pDeer->GetPlayer()->GetSteeringEntity() );
			m_pDeer->GetBehaviors()->PursuitOn();

			/*m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
			m_pDeer->GetBehaviors()->SeekOn();*/

			m_pDeer->FaceTo(m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			if ( m_RunActionStateCallback.IsActionFinished() )
			{
				m_RunActionStateCallback.InitAction();
				m_RunActionStateCallback.StartAction();

				m_FirtsStepDone		= false;
				m_SecondStepDone	= false;
			}
			
			if ( m_RunActionStateCallback.IsActionInTime( 0.3f ) && !m_FirtsStepDone )
			{
				GetParticleEmitter("StepLeft")->EjectParticles();
				m_FirtsStepDone	= true;
			}

			if ( m_RunActionStateCallback.IsActionInTime( 0.5f ) && !m_SecondStepDone )
			{
				GetParticleEmitter("StepRight")->EjectParticles();
				m_SecondStepDone = true;
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					std::string l_State = "Pursuing";
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
					//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s pursuit...", m_pRabbit->GetName().c_str() );
				}
			#endif
		}
	}
	else
	{
		// Volvemos al reposo
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetIdleState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	}
}

void CDeerPursuitState::OnExit( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->PursuitOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();

	m_pDeer->GetBehaviors()->SeparationOff();
	m_pDeer->GetBehaviors()->CohesionOff();
	//m_pDeer->GetBehaviors()->AlignmentOff();

	CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerRun");

	/*GetParticleEmitter("StepRight")->StopEjectParticles();
	GetParticleEmitter("StepLeft")->StopEjectParticles();*/
}

bool CDeerPursuitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_Character);
		}

		m_pDeer->RestLife(50); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}
	return false;
}

// Devuelve el tiempo, la duración
void CDeerPursuitState::PlayRandomSound( void )
{
	int l_Num = BoostRandomHelper::GetInt(1,3);
	if ( l_Num == 1 )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerRun1");
		m_SoundDuration = 2.5f;
	}
	else if ( l_Num == 2)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerRun2");
		m_SoundDuration = 2.5f;
	}
	else if ( l_Num == 3)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerRun3");
		m_SoundDuration = 3.320f;
	}
}

void CDeerPursuitState::SetParticlePosition( CCharacter* _pCharacter )
{
	CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();

	Mat44f l_TransformMatrix		= m44fIDENTITY;
	Mat44f l_RotationMatrix			= m44fIDENTITY;
	Vect4f l_Rotation				= v3fZERO;
	Vect3f l_Translation			= v3fZERO;
	Mat44f l_AnimatedModelTransform = l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("Bip001 R Foot", l_Translation);
	l_pAnimatedModel->GetBoneRotation("Bip001 R Foot", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	GetParticleEmitter("StepLeft")->SetPosition( l_TransformMatrix.GetPos() );
	
	l_TransformMatrix			= m44fIDENTITY;
	l_RotationMatrix			= m44fIDENTITY;
	l_Rotation					= v3fZERO;
	l_Translation				= v3fZERO;
	l_AnimatedModelTransform	= l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("Bip001 L Foot", l_Translation);
	l_pAnimatedModel->GetBoneRotation("Bip001 L Foot", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	GetParticleEmitter("StepRight")->SetPosition( l_TransformMatrix.GetPos() );
}