#include "RabbitAttackState.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "Characters\Character.h"

#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitTiredState.h"
#include "RabbitDefenseState.h"
#include "RabbitStillAttackState.h"
#include "RabbitRunAttackState.h"

#include "RabbitHitAnimationState.h"
#include "RabbitIdle2AnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitAttackState::CRabbitAttackState( void )
	: CState		("CRabbitAttackState")
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pRabbit		( NULL )
{
}

CRabbitAttackState::CRabbitAttackState( const std::string &_Name )
	: CState		(_Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pRabbit		( NULL )
{
}


CRabbitAttackState::~CRabbitAttackState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	// Permite saber si el caracter ha realizado los golpes para cansarse. Ponemos a 2 el hits_done para obligar a que cuando entra en este estado haga una minima pausa antes de atacar
	// _CCharacter.hits_done = 0
	m_pRabbit->SetTotalHitsToBeTired(2);
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->SeekOff();
	/*m_pRabbit->GetBehaviors()->SeparationOn();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();*/

}

void CRabbitAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	if ( m_pRabbit->IsPlayerAtacable() ) 
	{
		if  ( m_pRabbit->IsFatigued() )
		{
			// print_logger (1, "fatigat després de fer n cops seguits...")
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetTiredState());
		}
		else if ( m_pRabbit->GetReceivedHitsXMinut() == m_pRabbit->GetTotalReceivedHitsXMinut() ) 
		{
			// print_logger (1, "hits x minut rebuts i per tant bloquejaré...")
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetDefenseState());
		}
		else 
		{
			std::string l_ActiveActionState = GetRandomAttackName();
			// print_logger ( 0, "Attack Random Sel·leccionat "..l_ActiveActionState ) 
				
			if ( l_ActiveActionState == STILL_ATTACK_STATE ) 
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetStillAttackState());
			}	
			else if ( l_ActiveActionState == RUN_ATTACK_STATE ) 
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetRunAttackState());
			}	
			else if ( l_ActiveActionState == DEFENSE_STATE ) 
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetDefenseState());
			}		
			// else if ( l_ActiveActionState == "jump" ) then
				// _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
			else if ( l_ActiveActionState == "go_in_to_fustrum" ) 
			{
				float l_Angle = 22.f;		// 22,5 graus de fustrum
				m_pRabbit->GoInToFrustrum(l_Angle, _ElapsedTime);
			}
		} 	// End fatigue
	}	
	// Si el player NO es atacable lo volvemos a preparar o a perseguir
	else 
	{
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetPreparedToAttack());
	}
}

void CRabbitAttackState::OnExit( CCharacter* _Character )
{
	m_pRabbit->GetBehaviors()->SeparationOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
}

bool CRabbitAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// _Character->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}

	return false;
}

// --------------------------------------------------------------------------------------------------------
//	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animación. También
// 						puede devolver si solo se mueve sin atacar para engañar un poco al player
// --------------------------------------------------------------------------------------------------------
std::string CRabbitAttackState::GetRandomAttackName(void)
{
	std::string l_Action;	

	int l_AttackType = BoostRandomHelper::GetInt(1,12);
	if ( l_AttackType == 1 ) 
		l_Action = STILL_ATTACK_STATE;
	else if ( l_AttackType == 2 ) 
		l_Action =  STILL_ATTACK_STATE;
	else if ( l_AttackType == 3 ) 
		l_Action = STILL_ATTACK_STATE;
	else if ( l_AttackType == 4 ) 
		l_Action = RUN_ATTACK_STATE;
	else if ( l_AttackType == 5 ) 
		l_Action = RUN_ATTACK_STATE;
	else if ( l_AttackType == 6 ) 
		l_Action = RUN_ATTACK_STATE;
	else if ( l_AttackType == 7 ) 
		l_Action = DEFENSE_STATE;
	else if ( l_AttackType == 8 ) 
		l_Action =  JUMP_STATE;
			
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