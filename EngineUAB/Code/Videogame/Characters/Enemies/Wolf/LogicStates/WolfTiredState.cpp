#include "WolfTiredState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

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
	, m_pWolf		( NULL )
{
}

CWolfTiredState::CWolfTiredState( const std::string &_Name )
	: CState		(_Name)
	, m_ActionTime	( CActionStateCallback( 0.2f, 0.5f ) )
	, m_pWolf		( NULL )
{
}


CWolfTiredState::~CWolfTiredState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfTiredState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	m_ActionTime.StartAction();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Fatigued");
		}
	#endif
}

void CWolfTiredState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}
	
	if ( m_ActionTime.IsActionFinished() ) 
	{
		// restauramos la variable que determina si está cansado
		m_pWolf->SetHitsDone(0);
		m_pWolf->GetLogicFSM()->RevertToPreviousState();
	}

	m_ActionTime.Update(_ElapsedTime);

	// Mentre espero miro al player
	m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
}


void CWolfTiredState::OnExit( CCharacter* _Character )
{
}

bool CWolfTiredState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_Character);
		}

		m_pWolf->RestLife(1000); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
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