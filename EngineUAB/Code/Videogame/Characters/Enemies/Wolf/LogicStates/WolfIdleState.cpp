#include "WolfIdleState.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdle2AnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfIdleState::CWolfIdleState( void )
	: CState		("CWolfIdleState")
	, m_ActionTime  ( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}

CWolfIdleState::CWolfIdleState( const std::string &_Name )
	: CState		(_Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}


CWolfIdleState::~CWolfIdleState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfIdleState::OnEnter( CCharacter* _Character )
{
	m_pWolf = dynamic_cast<CWolf*> (_Character);
	m_ActionTime.StartAction();
}

void CWolfIdleState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	if ( m_pWolf->IsPlayerDetected() ) 
	{
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetPursuitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
	}
	else
	{
		if ( m_ActionTime.IsActionFinished() )
		{
			// pillamos la animación idle que deseemos
			if ( BoostRandomHelper::GetInt(1, 4) == 1 ) 
			{
				//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pWolf->GetIdleAnimationState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			}
			else
			{
				//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pWolf->GetIdle2AnimationState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdle2AnimationState());
			}

			// pillamos un tiempo de ejecución aleatorio
			m_ActionTime.StartAction();
		}
		else
		{
			m_ActionTime.Update(_ElapsedTime);
		}
	}

	// Reseteamos la velocidad del enemigo
	_Character->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	_Character->MoveTo2(_Character->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CWolfIdleState::OnExit( CCharacter* _Character )
{
}

bool CWolfIdleState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// _Character->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		_Character->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}

	return false;
}