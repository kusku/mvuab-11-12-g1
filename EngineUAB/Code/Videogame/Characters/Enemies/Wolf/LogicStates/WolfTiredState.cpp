#include "RabbitTiredState.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitHitState.h"

#include "RabbitIdleAnimationState.h"
#include "RabbitHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfTiredState::CWolfTiredState( void )
	: CState		("CWolfTiredState")
	, m_ActionTime	( CActionStateCallback( 0.2f, 0.5f ) )
	, m_pRabbit		( NULL )
{
}

CWolfTiredState::CWolfTiredState( const std::string &_Name )
	: CState		(_Name)
	, m_ActionTime	( CActionStateCallback( 0.2f, 0.5f ) )
	, m_pRabbit		( NULL )
{
}


CWolfTiredState::~CWolfTiredState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfTiredState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	m_ActionTime.StartAction();
}

void CWolfTiredState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}
	
	if ( m_ActionTime.IsActionFinished() ) 
	{
		// restauramos la variable que determina si está cansado
		m_pRabbit->SetHitsDone(0);
		m_pRabbit->GetLogicFSM()->RevertToPreviousState();
	}

	m_ActionTime.Update(_ElapsedTime);

	// Mentre espero miro al player
	_Character->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
}


void CWolfTiredState::OnExit( CCharacter* _Character )
{
}

bool CWolfTiredState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}

	return false;
}

std::string CWolfTiredState::GetRandomAttackName( void )
{
	int l_AttackType = BoostRandomHelper::GetInt(1, 12);
	std::string l_Result;

	if ( l_AttackType == 1 ) 
		l_Result = "attack_1";
	else if  ( l_AttackType == 2 ) 
		l_Result = "attack_1";
	else if  ( l_AttackType == 3 ) 
		l_Result = "attack_1";
	else if  ( l_AttackType == 4 ) 
		l_Result = "attack_2";
	else if  ( l_AttackType == 5 ) 
		l_Result = "attack_2";
	else if  ( l_AttackType == 6 ) 
		l_Result = "attack_2";
	else if  ( l_AttackType == 7 ) 
		l_Result = "defense";
	else if  ( l_AttackType == 8 ) 
		l_Result = "jump";
			
	// Damos la opción de tener más probabilidades de ir al fustrum que no atacar
	else if  ( l_AttackType == 9 ) 
		l_Result = "go_in_to_fustrum";
	else if  ( l_AttackType == 10 )
		l_Result = "go_in_to_fustrum";
	else if  ( l_AttackType == 11 ) 
		l_Result = "go_in_to_fustrum";
	else if  ( l_AttackType == 12 ) 
		l_Result = "go_in_to_fustrum";

	return l_Result;
}