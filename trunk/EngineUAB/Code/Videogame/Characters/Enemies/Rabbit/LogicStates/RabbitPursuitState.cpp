#include "RabbitPursuitState.h"
#include "RabbitIdleState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitHitState.h"

#include "SoundManager.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

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

#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitPursuitState::CRabbitPursuitState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CRabbitPursuitState")
	, m_pRabbit				( NULL )
	, m_CurrentSoundToPlay	( 1 )
	, m_ActionStateCallback	( 0, 0 )
{
}

CRabbitPursuitState::CRabbitPursuitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pRabbit				( NULL )
	, m_CurrentSoundToPlay	( 1 )
	, m_ActionStateCallback	( 0, 0 )
{
}

CRabbitPursuitState::~CRabbitPursuitState( void )
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

void CRabbitPursuitState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( !m_pRabbit->GetPlayerHasBeenReached() )
	{
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Rabbit_Run" );
		//PlaySequencialSound (m_CurrentSoundToPlay);
		//m_ActionStateCallback.InitAction(0, m_SoundDuration);w
		//m_ActionStateCallback.StartAction();
	}

	/*m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->SeekOn();*/
		
	m_pRabbit->GetBehaviors()->GetPursuit()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pRabbit->GetPlayer()->GetSteeringEntity() );
	m_pRabbit->GetBehaviors()->PursuitOn();
		
	m_pRabbit->GetBehaviors()->SeparationOn();
	m_pRabbit->GetBehaviors()->CohesionOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();
}

void CRabbitPursuitState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();

	if ( m_pRabbit->IsPlayerDetected() ) 
	{
		if ( m_pRabbit->IsEnemyPreparedToAttack() ) 
		{
			// Reseteamos la velocidad del enemigo y cambiamos a un estado que prepara para el ataque
			/*m_pRabbit->GetBehaviors()->PursuitOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetPreparedToAttack());
		}
		else
		{
			// Seguimos persiguiendo...
			m_pRabbit->GetBehaviors()->GetPursuit()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
			m_pRabbit->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pRabbit->GetPlayer()->GetSteeringEntity() );
			m_pRabbit->GetBehaviors()->PursuitOn();

			/*m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
			m_pRabbit->GetBehaviors()->SeekOn();*/

			m_pRabbit->FaceTo(m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			// Tratamos el sonido
			//if ( m_ActionStateCallback.IsActionFinished() )
			//{
			//	m_CurrentSoundToPlay++;		// vamos al siguiente sonido
			//	if ( m_CurrentSoundToPlay > 8 )
			//	{
			//		m_CurrentSoundToPlay = 1;
			//	}

			//	PlaySequencialSound(m_CurrentSoundToPlay);
			//	m_ActionStateCallback.InitAction( 0.f, m_SoundDuration );
			//	m_ActionStateCallback.StartAction();
			//}
			//else 
			//{
			//	m_ActionStateCallback.Update(_ElapsedTime);
			//}

			#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				std::string l_State = "Pursuing";
				CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
			}
			#endif
		}
	}
	else
	{
		// Volvemos al reposo
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	}
}


void CRabbitPursuitState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();

	m_pRabbit->GetBehaviors()->SeparationOff();
	m_pRabbit->GetBehaviors()->CohesionOff();
	//_pCharacter->GetBehaviors()->AlignmentOff();

	CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Stop_EFX_Rabbit_Run");
}

bool CRabbitPursuitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
		}

		m_pRabbit->RestLife(1000); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		return true;
	}
	return false;
}

