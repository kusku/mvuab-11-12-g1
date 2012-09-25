#include "RabbitHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "SoundManager.h"
#include "Utils\Timer.h"
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
	: CState						(_pCharacter, "CRabbitHitState")
	, m_pRabbit						( NULL )
	, m_pActionStateCallback		( 0.f, 1.f )
	, m_pAnimationCallback			( NULL )
	, m_IsCommingFromTired			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_HIT_STATE);
}

CRabbitHitState::CRabbitHitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState						(_pCharacter, _Name)
	, m_pRabbit						( NULL )
	, m_pActionStateCallback		( 0.f, 1.f )
	, m_pAnimationCallback			( NULL )
	, m_IsCommingFromTired			( false )
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
		m_pAnimationCallback->Init();
		m_pAnimationCallback->StartAnimation();
	
		//PlayRandomSound();
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Pain");

		// Aprovecho esta variable para calcular el tiempo de duración del desplazamiento
		m_ActionDuration = m_pRabbit->GetProperties()->GetHitRecoilDistance()/m_pRabbit->GetProperties()->GetHitRecoilSpeed() * CORE->GetTimer()->GetElapsedTime();
		m_pActionStateCallback.InitAction(0, m_ActionDuration); 
		m_pActionStateCallback.StartAction();

		//m_pActionStateCallback.InitAction(0.f, m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_HIT_STATE));
		//m_pActionStateCallback.StartAction();
		
		// --- Para la gestión del retroceso ---
		m_OldMaxSpeed = m_pRabbit->GetProperties()->GetMaxSpeed();
		m_pRabbit->GetProperties()->SetMaxSpeed(m_pRabbit->GetProperties()->GetHitRecoilSpeed());

		m_HitDirection = m_pRabbit->GetSteeringEntity()->GetFront();
		m_HitDirection.Normalize();
		m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));		
		
		m_MaxHitDistance = m_pRabbit->GetProperties()->GetHitRecoilDistance();
		m_InitialHitPoint = m_pRabbit->GetPosition();
		// ---------------------------------------

		// Metemos sangre!!
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
	}
	
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

	// Actualizamos la posición
	UpdateImpact(_pCharacter);

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
	if ( m_pActionStateCallback.IsActionFinished() )
	{
		/*m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());*/

		// Retrocedemos
		CProperties * l_Properties = m_pRabbit->GetProperties();
		m_pRabbit->GetBehaviors()->SeekOn();
		Vect3f l_Front = m_pRabbit->GetSteeringEntity()->GetFront();
		l_Front.Normalize();
		l_Front = l_Front.RotateY(mathUtils::PiTimes(1.f));
		l_Front = m_pRabbit->GetSteeringEntity()->GetPosition() + l_Front * l_Properties->GetHitRecoilSpeed();
		//m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(l_Front);

		if ( m_pRabbit->IsAlive() ) 
		{
			// Obligo a descansar entre unos segundos
			float l_MaxTimeInTired = BoostRandomHelper::GetFloat(l_Properties->GetMinTiredTimeAfterAttack(), l_Properties->GetMaxTiredTimeAfterAttack());
			m_RecoverMinTiredTime = m_pRabbit->GetTiredState()->GetMinTiredTime();
			m_RecoverMaxTiredTime = m_pRabbit->GetTiredState()->GetMaxTiredTime();
			m_pRabbit->GetTiredState()->SetTiredTime(0.f, l_MaxTimeInTired);
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetTiredState());
			m_IsCommingFromTired = true;
		}
	}
	else
	{
		m_pActionStateCallback.Update(_ElapsedTime);

		// Gestiono el retroceso del hit
		float l_Distance = m_pRabbit->GetPosition().Distance(m_InitialHitPoint);
		if ( l_Distance >= m_MaxHitDistance ) 
		{
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		} 
		else
		{
			//m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			//m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(l_Front);
			//m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pRabbit->MoveTo2(m_HitDirection, _ElapsedTime );
		}
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

void CRabbitHitState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("RabbitBloodDust",	_pCharacter->GetName() + "_RabbitBloodDust")->EjectParticles();
	GetParticleEmitterInstance("RabbitBlood",		_pCharacter->GetName() + "_RabbitBlood")->EjectParticles();
}

void CRabbitHitState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "RabbitBloodDust",	  _pCharacter->GetName() +   "_RabbitBloodDust",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitBlood",		  _pCharacter->GetName() +   "_RabbitBlood",	"", l_Pos);
}

void CRabbitHitState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBloodDust",	_pCharacter->GetName() + "_RabbitBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBlood",		_pCharacter->GetName() + "_RabbitBlood")->StopEjectParticles();
}