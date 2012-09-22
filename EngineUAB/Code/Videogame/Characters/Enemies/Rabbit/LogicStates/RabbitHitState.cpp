#include "RabbitHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "SoundManager.h"
#include "Core.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\Telegram.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "Characters\StatesDefs.h"

#include "RabbitIdleState.h"
#include "RabbitTiredState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitHitState::CRabbitHitState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CRabbitHitState")
	, m_pRabbit				( NULL )
	, m_pActionState		( 0.f, 1.f )
	, m_pAnimationCallback	( NULL )
	, m_IsCommingFromTired	( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_HIT_STATE);
}

CRabbitHitState::CRabbitHitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pRabbit				( NULL )
	, m_pActionState		( 0.f, 1.f )
	, m_pAnimationCallback	( NULL )
	, m_IsCommingFromTired	( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_HIT_STATE);
}


CRabbitHitState::~CRabbitHitState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitHitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	 
	// Si volvemos de haber recibido y después de estar cansados nos salimos.
	if ( m_IsCommingFromTired ) 
	{
		m_pRabbit->GetTiredState()->SetTiredTime(m_RecoverMinTiredTime, m_RecoverMaxTiredTime);	// Recuperamos el tiempo que teneniamos por defecto asignado al estado TIRED
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
		m_IsCommingFromTired = false;
	}
	// Si entramos por primera vez ejecutaremos el hit normal
	else 
	{
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		m_pAnimationCallback->Init();
		m_pAnimationCallback->StartAnimation();
	
		//PlayRandomSound();
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Pain");

		m_pActionState.InitAction(0, m_SoundDuration);
		m_pActionState.StartAction();

		//m_pActionState.InitAction(0.f, m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_HIT_STATE));
		//m_pActionState.StartAction();
	}
	
	// Ahora debemos actualizar las partículas
	UpdateParticlesPositions(m_pRabbit);

	// Gestión de partículas
	//GetParticleEmitter(m_pRabbit->GetName() + "_BloodSplash")->EjectParticles();
	
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = DEER_HIT_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif
}

void CRabbitHitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pRabbit->GetLogicFSM()->RevertToPreviousState();
			m_pRabbit->GetGraphicFSM()->RevertToPreviousState();
		}
	}
	else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	// Solo hago la acción si estoy dentro de la distancia de impacto
	if ( m_pActionState.IsActionFinished() )
	{
		/*m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());*/

		// Retrocedemos
		m_pRabbit->GetBehaviors()->SeekOn();
		Vect3f l_Front = m_pRabbit->GetSteeringEntity()->GetFront();
		l_Front.Normalize();
		l_Front = l_Front.RotateY(mathUtils::PiTimes(1.f));
		l_Front = m_pRabbit->GetSteeringEntity()->GetPosition() + l_Front * 3.f;
		//m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(l_Front);

		if ( m_pRabbit->IsAlive() ) 
		{
			// Obligo a descansar entre unos segundosw
			float l_MaxTimeInTired = BoostRandomHelper::GetFloat(3, 5);
			m_RecoverMinTiredTime = m_pRabbit->GetTiredState()->GetMinTiredTime();
			m_RecoverMaxTiredTime = m_pRabbit->GetTiredState()->GetMaxTiredTime();
			m_pRabbit->GetTiredState()->SetTiredTime(0.f, l_MaxTimeInTired);
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetTiredState());
			m_IsCommingFromTired = true;
		}
		//else
		/*{
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetDeathState());
		}*/
	}
	else
	{
		m_pActionState.Update(_ElapsedTime);
	}
}

void CRabbitHitState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if (m_pRabbit) 
	{
		//GetParticleEmitter(_pCharacter->GetName() + "_BloodSplash")->StopEjectParticles();
	}
}

bool CRabbitHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	return false;
}

// Devuelve el tiempo, la duración
void CRabbitHitState::PlayRandomSound( void )
{
	int l_Num = BoostRandomHelper::GetInt(1,4);
	if ( l_Num == 1 )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains1");
		m_SoundDuration = 0.858f;
	}
	else if ( l_Num == 2)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains2");
		m_SoundDuration = 0.817f;
	}
	else if ( l_Num == 3)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains3");
		m_SoundDuration = 0.851f;
	}
	else if ( l_Num == 4)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains4");
		m_SoundDuration = 0.483f;
	}
	else if ( l_Num == 5)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains5");
		m_SoundDuration = 0.637f;
	}
	else if ( l_Num == 6)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains6");
		m_SoundDuration = 0.865f;
	}
	else if ( l_Num == 7)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitPains7");
		m_SoundDuration = 0.717f;
	}
}

void CRabbitHitState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, _pCharacter->GetName() + "_BloodSplash" , "",_pCharacter->GetPosition() + _pCharacter->GetFront()  );
}

void CRabbitHitState::SetParticlePosition( CCharacter* _pCharacter, const std::string &_ParticlesName, const std::string &_Bone, const Vect3f &_Position )
{
	if ( _Bone.compare( "" ) != 0 )
	{
		CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();

		Mat44f l_TransformMatrix		= m44fIDENTITY;
		Mat44f l_RotationMatrix			= m44fIDENTITY;
		Vect4f l_Rotation				= v3fZERO;
		Vect3f l_Translation			= v3fZERO;
		Mat44f l_AnimatedModelTransform = l_pAnimatedModel->GetTransform();

		l_pAnimatedModel->GetBonePosition(_Bone, l_Translation);
		l_pAnimatedModel->GetBoneRotation(_Bone, l_Rotation);

		l_TransformMatrix.Translate(l_Translation);
		l_RotationMatrix.SetFromQuaternion(l_Rotation);

		l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

		//GetParticleEmitter(_ParticlesName)->SetPosition( l_TransformMatrix.GetPos() );
	}
	else 
	{
		//GetParticleEmitter(_ParticlesName)->SetPosition( _Position );
	}
}