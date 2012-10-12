#include "WolfHitState.h"
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

#include "Characters\Enemies\Wolf\Wolf.h"
#include "Characters\StatesDefs.h"

#include "WolfTiredState.h"
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

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
CWolfHitState::CWolfHitState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CWolfHitState")
	, m_pWolf				( NULL )
	, m_pActionStateCallback( 0.f, 1.f )
	, m_pAnimationCallback	( NULL )
	, m_IsCommingFromTired	( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_HIT_STATE);
}

CWolfHitState::CWolfHitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_pActionStateCallback( 0.f, 1.f )
	, m_pAnimationCallback	( NULL )
	, m_IsCommingFromTired	( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_HIT_STATE);
}


CWolfHitState::~CWolfHitState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfHitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	// Si volvemos de haber recibido y después de estar cansados nos salimos.
	if ( m_IsCommingFromTired && !m_DoubleHit ) 
	{
		m_pWolf->GetTiredState()->SetTiredTime(m_RecoverMinTiredTime, m_RecoverMaxTiredTime);	// Recuperamos el tiempo que teneniamos por defecto asignado al estado TIRED
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
		m_IsCommingFromTired = false;
	}
	// Si entramos por primera vez ejecutaremos el hit normal
	else 
	{
		m_pAnimationCallback->Init();
		m_pAnimationCallback->StartAnimation();
	
		// Restamos la vida y comprobamos i podemos ir a otros de howl
		m_pWolf->RestLife(m_pEnemy->GetProperties()->GetStrong()); 
	
		// Miro si hemos llegado a un nivel de vida para convocar enemigos
		if ( !m_pWolf->GetCanHowlForEnemies() && m_pWolf->TestIfCanHowlForEnemies() )
		{
			m_pWolf->SetCanHowlForEnemies(true);
		}

		// Miro si hemos llegado a un nivel de vida para revivirnos
		if ( !m_pWolf->GetCanHowlForLife() && m_pWolf->TestIfCanHowlForLife() )
		{
			m_pWolf->SetCanHowlForLife(true);
		}

		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Pain");

		//// Aprovecho esta variable para calcular el tiempo de duración del desplazamiento
		//m_ActionDuration = m_pWolf->GetProperties()->GetHitRecoilDistance()/m_pWolf->GetProperties()->GetHitRecoilSpeed() * CORE->GetTimer()->GetElapsedTime();
		//m_pActionStateCallback.InitAction(0, m_ActionDuration); 
		//m_pActionStateCallback.StartAction();

		m_ActionDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_HIT_STATE);
		m_pActionStateCallback.InitAction(0.f, m_ActionDuration);
		m_pActionStateCallback.StartAction();

		// --- Para la gestión del retroceso ---
		CalculateRecoilDirection(m_pWolf);
		m_DoubleHit = false;
		// ---------------------------------------
		
		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->GetBehaviors()->SeekOff();

		// Gestión de partículas. Metemos sangre!!
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
	}

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = WOLF_HIT_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfHitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	// Actualizamos la posición
	UpdateImpact(_pCharacter);
		
	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pWolf->GetLogicFSM()->RevertToPreviousState();
			m_pWolf->GetGraphicFSM()->RevertToPreviousState();
		}
	}*/
	/*else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	if ( m_pActionStateCallback.IsActionFinished() )
	{
		if ( m_pWolf->IsAlive() ) 
		{
			// Obligo a descansar entre unos segundos
			CProperties * l_Properties = m_pWolf->GetProperties();
			float l_MaxTimeInTired = BoostRandomHelper::GetFloat(l_Properties->GetMinTiredTimeAfterAttack(), l_Properties->GetMaxTiredTimeAfterAttack());
			m_RecoverMinTiredTime = m_pWolf->GetTiredState()->GetMinTiredTime();
			m_RecoverMaxTiredTime = m_pWolf->GetTiredState()->GetMaxTiredTime();
			m_pWolf->GetTiredState()->SetTiredTime(0.f, l_MaxTimeInTired);
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetTiredState());
			m_IsCommingFromTired = true;
		}
	}
	else
	{
		// En caso que recibamos otro hit mientras estemos en este estado ampliamos el recorrido
		if ( m_DoubleHit )
		{
			CalculateRecoilDirection(m_pWolf);
			LOGGER->AddNewLog(ELL_WARNING, "Double hit!!");
			// Esto permite que ya no reste vida ni se recalcule nada
			m_DoubleHit = false;
		}

		m_pActionStateCallback.Update(_ElapsedTime);
		
		// Gestiono el retroceso del hit
		float l_Distance = m_pWolf->GetPosition().Distance(m_InitialHitPoint);
		if ( l_Distance >= m_MaxHitDistance ) 
		{
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		} 
		else
		{
			m_pWolf->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}
}

void CWolfHitState::OnExit( CCharacter* _pCharacter )
{
	if (!_pCharacter) 
	{
		return;
	}

	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	// Reseteamos la velocidad y ya no nos movemos
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(_pCharacter->GetProperties()->GetMaxSpeed());
	StopImpact(_pCharacter);
	//m_pWolf->SetLocked(false);
}

bool CWolfHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_DoubleHit = true;
		return true;
	}
	return false;
}

void CWolfHitState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfBloodSplash", _pCharacter->GetName() + "_WolfBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("WolfBloodDust",	  _pCharacter->GetName() + "_WolfBloodDust")->EjectParticles();
	GetParticleEmitterInstance("WolfBlood",		  _pCharacter->GetName() + "_WolfBlood")->EjectParticles();
}

void CWolfHitState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "WolfBloodSplash", _pCharacter->GetName() + "_WolfBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "WolfBloodDust",	_pCharacter->GetName() + "_WolfBloodDust",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "WolfBlood",		_pCharacter->GetName() + "_WolfBlood",	"", l_Pos);
}

void CWolfHitState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfBloodSplash", _pCharacter->GetName() + "_WolfBloodSplash")->StopEjectParticles();
	GetParticleEmitterInstance("WolfBloodDust",	  _pCharacter->GetName() + "_WolfBloodDust")->StopEjectParticles();
	GetParticleEmitterInstance("WolfBlood",		  _pCharacter->GetName() + "_WolfBlood")->StopEjectParticles();
}

void CWolfHitState::UpdateParameters( STelegram& _Message )
{
	m_Message = _Message;

	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);
}

void CWolfHitState::CalculateRecoilDirection( CCharacter * _pCharacter ) 
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		if (!m_pWolf)
			return;
	}

	// Calculamos la dirección y fuerza de retroceso a partir del tipo de mensaje recibido
	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);

	CProperties * l_Properties = m_pWolf->GetProperties();
	m_pWolf->FaceTo(m_pWolf->GetPlayer()->GetPosition(), CORE->GetTimer()->GetElapsedTime());
	m_MaxHitSpeed = l_Properties->GetHitRecoilSpeed();
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_MaxHitSpeed);
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance();
	m_InitialHitPoint = m_pWolf->GetPosition();
	
	// Cojemos la dirección que se recibe del atacante 
	Vect3f l_EnemyFront = m_pEnemy->GetSteeringEntity()->GetHeading();
	Vect3f l_OwnFront = m_pWolf->GetFront();
	//m_pWolf->SetLocked(true);
	
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
