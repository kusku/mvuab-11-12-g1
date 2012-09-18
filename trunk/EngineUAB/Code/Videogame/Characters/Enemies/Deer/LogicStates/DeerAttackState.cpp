#include "DeerAttackState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "Characters\Character.h"

#include "DeerPursuitState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerTiredState.h"
#include "DeerDefenseState.h"
#include "DeerStillAttackState.h"
#include "DeerRunAttackState.h"
#include "DeerHitState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerDefenseAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerAttackState::CDeerAttackState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CDeerAttackState")
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pDeer		( NULL )
{
}

CDeerAttackState::CDeerAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pDeer		( NULL )
{
}


CDeerAttackState::~CDeerAttackState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Permite saber si el caracter ha realizado los golpes para cansarse. Ponemos a 2 el hits_done para obligar a que cuando entra en este estado haga una minima pausa antes de atacar
	// _CCharacter.hits_done = 0
	m_pDeer->SetTotalHitsToBeTired(2);
	m_pDeer->GetBehaviors()->PursuitOff();
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->SeparationOn();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOn();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOn();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	if ( m_pDeer->IsPlayerAtacable() ) 
	{
		// si venimos del anterior estado haremos una pausa
		if  ( m_pDeer->IsFatigued() )
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CDeerAttackState::Execute->Is fatigued");
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetTiredState());
		}
		else if ( m_pDeer->GetReceivedHitsXMinut() == m_pDeer->GetTotalReceivedHitsXMinut() ) 
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "CDeerAttackState::Execute->hits x minut rebuts i per tant bloquejaré...");
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetDefenseState());
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetDefenseAnimationState());
		}
		else 
		{
			std::string l_ActiveActionState = GetRandomAttackName();
			LOGGER->AddNewLog(ELL_INFORMATION, "CDeerAttackState::Execute->Attack Random Sel·leccionat %s", l_ActiveActionState.c_str());
				
			if ( l_ActiveActionState == DEER_STILL_ATTACK_STATE ) 
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetStillAttackState());
			}	
			else if ( l_ActiveActionState == DEER_RUN_ATTACK_STATE ) 
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetRunAttackState());
			}	
			else if ( l_ActiveActionState == DEER_DEFENSE_STATE ) 
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetDefenseState());
			}	
			// else if ( l_ActiveActionState == "jump" ) then
				// _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
			else if ( l_ActiveActionState == "go_in_to_fustrum" ) 
			{
				float l_Angle = 22.f;		// 22,5 graus de fustrum
				//m_pDeer->GoIntoCameraFrustum(l_Angle, _ElapsedTime);
			}
		} 	// End fatigue
	}	
	// Si el player NO es atacable lo volvemos a preparar o a perseguir
	else 
	{
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetPreparedToAttack());
	}
}

void CDeerAttackState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_pDeer->GetBehaviors()->SeparationOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();
}

bool CDeerAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_Character);
		}

		m_pDeer->RestLife(50); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}

// --------------------------------------------------------------------------------------------------------
//	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animación. También
// 						puede devolver si solo se mueve sin atacar para engañar un poco al player
// --------------------------------------------------------------------------------------------------------
std::string CDeerAttackState::GetRandomAttackName(void)
{
	std::string l_Action;	

	int l_AttackType = BoostRandomHelper::GetInt(1,12);
	if ( l_AttackType == 1 ) 
		l_Action = DEER_STILL_ATTACK_STATE;
	else if ( l_AttackType == 2 ) 
		l_Action =  DEER_STILL_ATTACK_STATE;
	else if ( l_AttackType == 3 ) 
		l_Action = DEER_STILL_ATTACK_STATE;
	else if ( l_AttackType == 4 ) 
		l_Action = DEER_RUN_ATTACK_STATE;
	else if ( l_AttackType == 5 ) 
		l_Action = DEER_RUN_ATTACK_STATE;
	else if ( l_AttackType == 6 ) 
		l_Action = DEER_RUN_ATTACK_STATE;
	else if ( l_AttackType == 7 ) 
		l_Action = DEER_DEFENSE_STATE;
	else if ( l_AttackType == 8 ) 
		l_Action =  DEER_JUMP_STATE;
			
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