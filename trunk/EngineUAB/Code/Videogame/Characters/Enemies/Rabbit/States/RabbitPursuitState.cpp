#include "RabbitPursuitState.h"
#include "RabbitIdleState.h"
#include "RabbitPreparedToAttackState.h"

#include "RabbitIdleAnimationState.h"
#include "RabbitHitAnimationState.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitPursuitState::CRabbitPursuitState( void )
	: m_pRabbit		( NULL )
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
void CRabbitPursuitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	if ( m_pRabbit->IsPlayerDetected() ) 
	{
		if ( m_pRabbit->IsEnemyPreparedToAttack() ) 
		{
			// Reseteamos la velocidad del enemigo
			//_CCharacter.behaviors:pursuit_off()
			_Character->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			_Character->GetLogicFSM()->ChangeState(m_pRabbit->GetPreparedToAttack());
		}
		else
		{
			_Character->FaceTo(_Character->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			_Character->MoveTo2(_Character->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
		}
	}
	else
	{
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetIdleState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	}
}

void CRabbitPursuitState::OnEnter( CCharacter* _Character )
{}

void CRabbitPursuitState::OnExit( CCharacter* _Character )
{
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