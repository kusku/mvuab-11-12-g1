#include "RabbitPursuitState.h"
#include "RabbitIdleState.h"
#include "RabbitPreparedToAttackState.h"

#include "RabbitIdleAnimationState.h"
#include "RabbitHitAnimationState.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Pursuit.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitPursuitState::CRabbitPursuitState( void )
	: CState	("CRabbitPursuitState")
	, m_pRabbit	( NULL )
{
}

CRabbitPursuitState::CRabbitPursuitState( const std::string &_Name )
	: CState		(_Name)
	, m_pRabbit		( NULL )
{
}

CRabbitPursuitState::~CRabbitPursuitState( void )
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

void CRabbitPursuitState::OnEnter( CCharacter* _Character )
{
	// _Character->GetBehaviors()->GetSeek()->SetTarget(_Character->GetPlayer()->GetPosition());
	// _Character->GetBehaviors()->SeekOn();
		
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	m_pRabbit->GetBehaviors()->GetPursuit()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( m_pRabbit->GetPlayer()->GetSteeringEntity() );
	m_pRabbit->GetBehaviors()->PursuitOff();
		
	// _Character->GetBehaviors()->separation_on()
	// _Character->GetBehaviors()->collision_avoidance_on()
	// _Character->GetBehaviors()->obstacle_wall_avoidance_on()
}

void CRabbitPursuitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}
	
	_Character->GetBehaviors()->PursuitOff();

	if ( _Character->IsPlayerDetected() ) 
	{
		if ( _Character->IsEnemyPreparedToAttack() ) 
		{
			// Reseteamos la velocidad del enemigo y cambiamos a un estado que prepara para el ataque
			m_pRabbit->GetBehaviors()->PursuitOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetPreparedToAttack());
		}
		else
		{
			// Seguimos persiguiendo...
			m_pRabbit->GetBehaviors()->GetPursuit()->SetTarget(_Character->GetPlayer()->GetPosition());
			m_pRabbit->GetBehaviors()->GetPursuit()->UpdateEvaderEntity( _Character->GetPlayer()->GetSteeringEntity() );
			m_pRabbit->GetBehaviors()->PursuitOn();

			m_pRabbit->FaceTo(_Character->GetSteeringEntity()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(_Character->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
		}
	}
	else
	{
		// Volvemos al reposo
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	}
}


void CRabbitPursuitState::OnExit( CCharacter* _Character )
{
	//_Character->GetBehaviors()->SeekOff();
	_Character->GetBehaviors()->PursuitOff();
	_Character->GetBehaviors()->CollisionAvoidanceOff();
	_Character->GetBehaviors()->ObstacleWallAvoidanceOff();

	//_Character->GetBehaviors()->SeparationOff();
	//_Character->GetBehaviors()->CohesionOff();
	//_Character->GetBehaviors()->AlignmentOff();
	
}

bool CRabbitPursuitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pRabbit->GetHitAnimationState());
		_Character->GetLogicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}
	return false;
}