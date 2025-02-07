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
	 
	// Si volvemos de haber recibido y despu�s de estar cansados nos salimos.
	if ( m_IsCommingFromTired && !m_DoubleHit ) 
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
	
		m_pRabbit->RestLife(m_pEnemy->GetProperties()->GetStrong()); 
		
		//PlayRandomSound();
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Pain");

		// Aprovecho esta variable para calcular el tiempo de duraci�n del desplazamiento
		//m_ActionDuration = m_pRabbit->GetProperties()->GetHitRecoilDistance()/m_pRabbit->GetProperties()->GetHitRecoilSpeed() * CORE->GetTimer()->GetElapsedTime();
		/*m_pActionStateCallback.InitAction(0, m_ActionDuration); 
		m_pActionStateCallback.StartAction();*/

		m_ActionDuration = m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_HIT_STATE);
		m_pActionStateCallback.InitAction(0.f, m_ActionDuration);
		m_pActionStateCallback.StartAction();
		
		// --- Para la gesti�n del retroceso ---
		CalculateRecoilDirection(m_pRabbit);
		m_DoubleHit = false;
		// ---------------------------------------

		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->GetBehaviors()->SeekOff();

		// Gesti�n de part�culas. Metemos sangre!!
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
	}
	
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = RABBIT_HIT_STATE;
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

	// Actualizamos la posici�n
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

	// Solo hago la acci�n si estoy dentro de la distancia de impacto
	if ( m_pActionStateCallback.IsActionFinished() )
	{
		if ( m_pRabbit->IsAlive() ) 
		{
			// Obligo a descansar entre unos segundos
			CProperties * l_Properties = m_pRabbit->GetProperties();
			float l_MaxTimeInTired = BoostRandomHelper::GetFloat(l_Properties->GetMinTiredTimeAfterAttack(), l_Properties->GetMaxTiredTimeAfterAttack());
			m_RecoverMinTiredTime = l_Properties->GetMinTiredTime();
			m_RecoverMaxTiredTime = l_Properties->GetMaxTiredTime();
			m_pRabbit->GetTiredState()->SetTiredTime(0.f, l_MaxTimeInTired);
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetTiredState());
			m_IsCommingFromTired = true;
		}
	}
	else
	{
		// En caso que recibamos otro hit mientras estemos en este estado ampliamos el recorrido
		if ( m_DoubleHit )
		{
			CalculateRecoilDirection(m_pRabbit);
			LOGGER->AddNewLog(ELL_WARNING, "Double hit!!");
			// Esto permite que ya no reste vida ni se recalcule nada
			m_DoubleHit = false;
		}

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
			m_pRabbit->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}
}

void CRabbitHitState::OnExit( CCharacter* _pCharacter )
{
	if ( _pCharacter == NULL )
	{
		return;
	}

	StopImpact(_pCharacter);	
	
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(_pCharacter->GetProperties()->GetMaxSpeed());
}

bool CRabbitHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_DoubleHit = true;
		return true;
	}
	return false;
}

void CRabbitHitState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("RabbitBloodDust",	_pCharacter->GetName() + "_RabbitBloodDust")->EjectParticles();
	GetParticleEmitterInstance("RabbitBlood",		_pCharacter->GetName() + "_RabbitBlood")->EjectParticles();

	// Los impactos que realiza la cape!!
	//GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->EjectParticles();
}

void CRabbitHitState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetSteeringEntity()->GetPosition();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	Vect3f l_Front = _pCharacter->GetFront();
	l_Front.RotateY(mathUtils::Deg2Rad(180.f));
	l_Pos += l_Front * 1.3f;

	SetParticlePosition(_pCharacter, "RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "RabbitBloodDust",	  _pCharacter->GetName() + "_RabbitBloodDust",	 "", l_Pos );
	SetParticlePosition(_pCharacter, "RabbitBlood",		  _pCharacter->GetName() + "_RabbitBlood",	"",		 l_Pos );
																				   
	// Los impactos que realiza la cape!!	
	//SetParticlePosition(_pCharacter, "CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1", "", l_Pos );
}

void CRabbitHitState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBloodSplash", _pCharacter->GetName() + "_RabbitBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBloodDust",	_pCharacter->GetName() + "_RabbitBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBlood",		_pCharacter->GetName() + "_RabbitBlood")->StopEjectParticles();

	// Los impactos que realiza la cape!!										   
	//GetParticleEmitterInstance("CaperucitaImpact1",		_pCharacter->GetName() + "_CaperucitaImpact1")->StopEjectParticles();
}

void CRabbitHitState::UpdateParameters( STelegram& _Message )
{
	m_Message = _Message;

	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);
}

void CRabbitHitState::CalculateRecoilDirection( CCharacter * _pCharacter ) 
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
		if (!m_pRabbit)
			return;
	}

	// Calculamos la direcci�n y fuerza de retroceso a partir del tipo de mensaje recibido
	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);

	CProperties * l_Properties = m_pRabbit->GetProperties();
	m_pRabbit->FaceTo(m_pRabbit->GetPlayer()->GetPosition(), CORE->GetTimer()->GetElapsedTime());
	m_MaxHitSpeed = l_Properties->GetHitRecoilSpeed();
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_MaxHitSpeed);
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance();
	m_InitialHitPoint = m_pRabbit->GetPosition();
	
	// Cojemos la direcci�n que se recibe del atacante 
	Vect3f l_EnemyFront = m_pEnemy->GetSteeringEntity()->GetHeading();
	Vect3f l_OwnFront = m_pRabbit->GetFront();
	//m_pRabbit->SetLocked(true);
	
	if ( m_Message.Msg == Msg_Attack )
	{
		// Si no tiene velocidad el player rotamos hacia el enemigo y reculamos en su direcci�n
		if ( _pCharacter->GetSteeringEntity()->GetSpeed() == 0 )
		{
			m_HitDirection = l_OwnFront;
			m_HitDirection = l_OwnFront.RotateY(mathUtils::PiTimes(1.f));	
			m_HitDirection *= m_MaxHitSpeed;
		}
		// Si tenemos los dos velocidad el vector resultante indicar� la direcci�n a seguir y recular
		else
		{
			// Direcci�n segun la suma de vectores
			m_HitDirection = l_EnemyFront + l_OwnFront;
		}
	}
	else if ( m_Message.Msg == Msg_Push )
	{
		m_HitDirection = l_EnemyFront;
	}
}