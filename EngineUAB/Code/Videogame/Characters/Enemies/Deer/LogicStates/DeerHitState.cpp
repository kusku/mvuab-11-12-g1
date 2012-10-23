#include "DeerHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
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

#include "Characters\Enemies\Deer\Deer.h"
#include "Characters\StatesDefs.h"

#include "DeerIdleState.h"
#include "DeerTiredState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeeridleAnimationState.h"

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
CDeerHitState::CDeerHitState( CCharacter* _pCharacter )
	: CState						(_pCharacter, "CDeerHitState")
	, m_pDeer						( NULL )
	, m_pActionStateCallback		( 0.f, 1.f )
	, m_pAnimationCallback			( NULL )
	, m_IsCommingFromTired			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),DEER_HIT_STATE);
}

CDeerHitState::CDeerHitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState						(_pCharacter, _Name)
	, m_pDeer						( NULL )
	, m_pActionStateCallback		( 0.f, 1.f )
	, m_pAnimationCallback			( NULL )
	, m_IsCommingFromTired			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),DEER_HIT_STATE);
}


CDeerHitState::~CDeerHitState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerHitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Si volvemos de haber recibido y después de estar cansados nos salimos.
	if ( m_IsCommingFromTired && !m_DoubleHit ) 
	{
		m_pDeer->GetTiredState()->SetTiredTime(m_RecoverMinTiredTime, m_RecoverMaxTiredTime);	// Recuperamos el tiempo que teneniamos por defecto asignado al estado TIRED
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
		m_IsCommingFromTired = false;
	}
	// Si entramos por primera vez ejecutaremos el hit normal
	else 
	{
		m_pAnimationCallback->Init();
		m_pAnimationCallback->StartAnimation();
	
		m_pDeer->RestLife(m_pEnemy->GetProperties()->GetStrong()); 
	
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Deer_Pain");

		//// Aprovecho esta variable para calcular el tiempo de duración del desplazamiento
		//m_ActionDuration = m_pDeer->GetProperties()->GetHitRecoilDistance()/m_pDeer->GetProperties()->GetHitRecoilSpeed() * CORE->GetTimer()->GetElapsedTime();
		//m_pActionStateCallback.InitAction(0, m_ActionDuration); 
		//m_pActionStateCallback.StartAction();

		m_ActionDuration = m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_HIT_STATE);
		m_pActionStateCallback.InitAction(0.f, m_ActionDuration);
		m_pActionStateCallback.StartAction();

		// --- Para la gestión del retroceso ---
		CalculateRecoilDirection(m_pDeer);
		m_DoubleHit = false;
		// ---------------------------------------

		m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pDeer->GetBehaviors()->SeekOff();
		
		// Gestión de partículas. Metemos sangre!!
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
	}

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = DEER_HIT_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerHitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Actualizamos la posición
	UpdateImpact(_pCharacter);
		
	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pDeer->GetLogicFSM()->RevertToPreviousState();
			m_pDeer->GetGraphicFSM()->RevertToPreviousState();
		}
	}*/
	/*else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	if ( m_pActionStateCallback.IsActionFinished() )
	{
		if ( m_pDeer->IsAlive() ) 
		{
			// Obligo a descansar entre unos segundos
			CProperties * l_Properties = m_pDeer->GetProperties();
			float l_MaxTimeInTired = BoostRandomHelper::GetFloat(l_Properties->GetMinTiredTimeAfterAttack(), l_Properties->GetMaxTiredTimeAfterAttack());
			m_RecoverMinTiredTime = l_Properties->GetMinTiredTime();
			m_RecoverMaxTiredTime = l_Properties->GetMaxTiredTime();
			m_pDeer->GetTiredState()->SetTiredTime(0.f, l_MaxTimeInTired);
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetTiredState());
			m_IsCommingFromTired = true;
		}
	}
	else
	{
		// En caso que recibamos otro hit mientras estemos en este estado ampliamos el recorrido
		if ( m_DoubleHit )
		{
			CalculateRecoilDirection(m_pDeer);
			LOGGER->AddNewLog(ELL_WARNING, "Double hit!!");
			// Esto permite que ya no reste vida ni se recalcule nada
			m_DoubleHit = false;
		}

		m_pActionStateCallback.Update(_ElapsedTime);
		
		// Gestiono el retroceso del hit
		float l_Distance = m_pDeer->GetPosition().Distance(m_InitialHitPoint);
		if ( l_Distance >= m_MaxHitDistance ) 
		{
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		} 
		else
		{
			m_pDeer->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}
}

void CDeerHitState::OnExit( CCharacter* _pCharacter )
{
	if (!_pCharacter) 
	{
		return;
	}

	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Reseteamos la velocidad y ya no nos movemos
	m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(_pCharacter->GetProperties()->GetMaxSpeed());
	StopImpact(_pCharacter);
}

bool CDeerHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_DoubleHit = true;
		return true;
	}
	return false;
}

void CDeerHitState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("DeerBloodSplash", _pCharacter->GetName() + "_DeerBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("DeerBloodDust",	  _pCharacter->GetName() + "_DeerBloodDust")->EjectParticles();
	GetParticleEmitterInstance("DeerBlood",		  _pCharacter->GetName() + "_DeerBlood")->EjectParticles();

	// Los impactos que realiza la cape!!
	//GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->EjectParticles();
}

void CDeerHitState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition(); 
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "DeerBloodSplash", _pCharacter->GetName() + "_DeerBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "DeerBloodDust",	_pCharacter->GetName() + "_DeerBloodDust",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "DeerBlood",		_pCharacter->GetName() + "_DeerBlood",	"", l_Pos);

	// Los impactos que realiza la cape!!
	//SetParticlePosition(_pCharacter, "CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1", "", l_Pos);
}

void CDeerHitState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("DeerBloodSplash", _pCharacter->GetName() + "_DeerBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("DeerBloodDust",	  _pCharacter->GetName() + "_DeerBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("DeerBlood",		  _pCharacter->GetName() + "_DeerBlood")->StopEjectParticles();

	// Los impactos que realiza la cape!!
	//GetParticleEmitterInstance("CaperucitaImpact1", _pCharacter->GetName() + "_CaperucitaImpact1")->StopEjectParticles();
}

void CDeerHitState::UpdateParameters( STelegram& _Message )
{
	m_Message = _Message;
	
	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);
}

void CDeerHitState::CalculateRecoilDirection( CCharacter * _pCharacter ) 
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
		if (!m_pDeer)
			return;
	}

	// Calculamos la dirección y fuerza de retroceso a partir del tipo de mensaje recibido
	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);

	CProperties * l_Properties = m_pDeer->GetProperties();
	m_pDeer->FaceTo(m_pDeer->GetPlayer()->GetPosition(), CORE->GetTimer()->GetElapsedTime());
	m_MaxHitSpeed = l_Properties->GetHitRecoilSpeed();
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_MaxHitSpeed);
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance();
	m_InitialHitPoint = m_pDeer->GetPosition();
	
	// Cojemos la dirección que se recibe del atacante 
	Vect3f l_EnemyFront = m_pEnemy->GetSteeringEntity()->GetHeading();
	Vect3f l_OwnFront = m_pDeer->GetFront();
	//m_pDeer->SetLocked(true);
	
	if ( m_Message.Msg == Msg_Attack )
	{
		// Si no tiene velocidad el player rotamos hacia el enemigo y reculamos en su dirección
		if ( _pCharacter->GetSteeringEntity()->GetSpeed() == 0 )
		{
			m_HitDirection = l_OwnFront;
			m_HitDirection = l_OwnFront.RotateY(mathUtils::PiTimes(1.f));	
			m_HitDirection *= m_MaxHitSpeed;
		}
		// Si tenemos los dos velocidad el vector resultante indicará la dirección a seguir y recular
		else
		{
			// Dirección segun la suma de vectores
			m_HitDirection = l_EnemyFront + l_OwnFront;
		}
	}
	else if ( m_Message.Msg == Msg_Push )
	{
		m_HitDirection = l_EnemyFront;
	}
}
