#include "WolfAttackState.h"
#include "Utils\BoostRandomHelper.h"
#include "Logger\Logger.h"
#include "Base.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "Characters\Character.h"

#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfTiredState.h"
#include "WolfDefenseState.h"
#include "WolfStillAttackState.h"
#include "WolfRunAttackState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfDefenseAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfAttackState::CWolfAttackState( void )
	: CState		("CWolfAttackState")
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}

CWolfAttackState::CWolfAttackState( const std::string &_Name )
	: CState		(_Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}


CWolfAttackState::~CWolfAttackState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	// Permite saber si el caracter ha realizado los golpes para cansarse. Ponemos a 2 el hits_done para obligar a que cuando entra en este estado haga una minima pausa antes de atacar
	// _CCharacter.hits_done = 0
	m_pWolf->SetTotalHitsToBeTired(2);
	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->SeparationOn();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Attack");
		}
	#endif
}

void CWolfAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	if ( m_pWolf->IsPlayerAtacable() ) 
	{
		if  ( m_pWolf->IsFatigued() )
		{
			// print_logger (1, "fatigat després de fer n cops seguits...")
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetTiredState());
		}
		else if ( m_pWolf->GetReceivedHitsXMinut() == m_pWolf->GetTotalReceivedHitsXMinut() ) 
		{
			//LOGGER->AddNewLog(ELL_INFORMATION, "CWolfAttackState::Execute->hits x minut rebuts i per tant bloquejaré...");
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetDefenseState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetDefenseAnimationState());
		}
		else 
		{
			std::string l_ActiveActionState = GetRandomAttackName();
			// print_logger ( 0, "Attack Random Sel·leccionat "..l_ActiveActionState ) 
				
			if ( l_ActiveActionState == WOLF_STILL_ATTACK_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetStillAttackState());
			}	
			else if ( l_ActiveActionState == WOLF_RUN_ATTACK_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetRunAttackState());
			}	
			else if ( l_ActiveActionState == WOLF_DEFENSE_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetDefenseState());
			}		
			// else if ( l_ActiveActionState == "jump" ) then
				// _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
			else if ( l_ActiveActionState == "go_in_to_fustrum" ) 
			{
				float l_Angle = 22.f;		// 22,5 graus de fustrum
				m_pWolf->GoInToFrustrum(l_Angle, _ElapsedTime);
			}

			#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s in some attack...", m_pWolf->GetName().c_str() );
			}
			#endif

		} 	// End fatigue
	}	
	// Si el player NO es atacable lo volvemos a preparar o a perseguir
	else 
	{
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetPreparedToAttack());
	}
}

void CWolfAttackState::OnExit( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->SeparationOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();
}

bool CWolfAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
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

// --------------------------------------------------------------------------------------------------------
//	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animación. También
// 						puede devolver si solo se mueve sin atacar para engañar un poco al player
// --------------------------------------------------------------------------------------------------------
std::string CWolfAttackState::GetRandomAttackName(void)
{
	std::string l_Action;	

	int l_AttackType = BoostRandomHelper::GetInt(1,12);
	if ( l_AttackType == 1 ) 
		l_Action = RABBIT_STILL_ATTACK_STATE;
	else if ( l_AttackType == 2 ) 
		l_Action =  RABBIT_STILL_ATTACK_STATE;
	else if ( l_AttackType == 3 ) 
		l_Action = RABBIT_STILL_ATTACK_STATE;
	else if ( l_AttackType == 4 ) 
		l_Action = RABBIT_RUN_ATTACK_STATE;
	else if ( l_AttackType == 5 ) 
		l_Action = RABBIT_RUN_ATTACK_STATE;
	else if ( l_AttackType == 6 ) 
		l_Action = RABBIT_RUN_ATTACK_STATE;
	else if ( l_AttackType == 7 ) 
		l_Action = RABBIT_DEFENSE_STATE;
	else if ( l_AttackType == 8 ) 
		l_Action =  RABBIT_JUMP_STATE;
			
	// Más probabilidades de ir al fustrum que no atacar
	else if ( l_AttackType == 9 ) 
		l_Action =  "go_in_to_fustrum";
	else if ( l_AttackType == 10 ) 
		l_Action =  "go_in_to_fustrum";
	else if ( l_AttackType == 11 ) 
		l_Action =  "go_in_to_fustrum";
	else if ( l_AttackType == 12 ) 
		l_Action =  "go_in_to_fustrum";

	return l_Action;
}