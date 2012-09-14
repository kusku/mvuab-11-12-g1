#include "DeerPursuitState.h"
#include "DeerIdleState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerHitState.h"
#include "SoundManager.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"

#include "Characters\Enemies\Deer\Deer.h"

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

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerPursuitState::CDeerPursuitState( void )
	: CState	("CDeerPursuitState")
	, m_pDeer	( NULL )
{
}

CDeerPursuitState::CDeerPursuitState( const std::string &_Name )
	: CState		(_Name)
	, m_pDeer		( NULL )
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

	CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerRun1");
}

void CDeerPursuitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->PursuitOff();

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

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Pursuing");
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
		return true;
	}
	return false;
}