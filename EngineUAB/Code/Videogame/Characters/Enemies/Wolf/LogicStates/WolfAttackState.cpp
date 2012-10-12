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
#include "WolfStillAttackComboState.h"
#include "WolfRunAttackState.h"
#include "WolfHitState.h"
#include "WolfHowlEnemiesState.h"
#include "WolfHowlLifeState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfDefenseAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlEnemiesAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlLifeAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfAttackState::CWolfAttackState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CWolfAttackState")
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}

CWolfAttackState::CWolfAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
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
void CWolfAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
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
			std::string l_State = "Attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
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
			if ( l_ActiveActionState == WOLF_STILL_ATTACK_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetStillAttackState());
			}	
			if ( l_ActiveActionState == WOLF_STILL_ATTACK_COMBO_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetStillAttackComboState());
			}	
			else if ( l_ActiveActionState == WOLF_RUN_ATTACK_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetRunAttackState());
			}	
			else if ( l_ActiveActionState == WOLF_DEFENSE_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetDefenseState());
			}
			else if ( l_ActiveActionState == WOLF_HOWL_ENEMIES_STATE ) 
			{
				// Si tenemos máxima vida y acaba de aparecer el lobo llamamos mis amigos.
				if ( m_pWolf->GetCanHowlForEnemies() )
				{
					m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetHowlEnemiesState());
					m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlEnemiesAnimationState());
					m_pWolf->SetCanHowlForEnemies(false);
					m_pWolf->AddNumberHowlEnemiesCalls();
					return;
				}
			}
			else if ( l_ActiveActionState == WOLF_HOWL_LIFE_STATE ) 
			{
				// Si le falta vida la recupera
				if ( m_pWolf->GetCanHowlForLife()  )
				{
					m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetHowlLifeState());
					m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlLifeAnimationState());
					m_pWolf->SetCanHowlForLife(false);
					m_pWolf->AddNumberHowlEnemiesCalls();
					return;
				}
			}
			// else if ( l_ActiveActionState == "jump" ) then
				// _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
			else if ( l_ActiveActionState == "go_in_to_frustum" ) 
			{
				float l_Angle = 22.f;		// 22,5 graus de fustrum
				m_pWolf->GoIntoCameraFrustum(l_Angle, _ElapsedTime);
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

void CWolfAttackState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->SeparationOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();
}

bool CWolfAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	return m_pWolf->CallHitState(_pCharacter, _Telegram);
}

// --------------------------------------------------------------------------------------------------------
//	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animación. También
// 						puede devolver si solo se mueve sin atacar para engañar un poco al player
// --------------------------------------------------------------------------------------------------------
std::string CWolfAttackState::GetRandomAttackName(void)
{
	std::string l_Action;	

	int l_AttackType = BoostRandomHelper::GetInt(1,24);
	if ( l_AttackType == 1 ) 
		l_Action = WOLF_STILL_ATTACK_STATE;
	else if ( l_AttackType == 2 ) 
		l_Action =  WOLF_STILL_ATTACK_STATE;
	else if ( l_AttackType == 3 ) 
		l_Action = WOLF_STILL_ATTACK_STATE;
	else if ( l_AttackType == 4 ) 
		l_Action = WOLF_STILL_ATTACK_STATE;
	else if ( l_AttackType == 5 ) 
		l_Action = WOLF_STILL_ATTACK_COMBO_STATE;
	else if ( l_AttackType == 6 ) 
		l_Action =  WOLF_STILL_ATTACK_COMBO_STATE;
	else if ( l_AttackType == 7 ) 
		l_Action = WOLF_STILL_ATTACK_COMBO_STATE;
	else if ( l_AttackType == 8 ) 
		l_Action = WOLF_RUN_ATTACK_STATE;
	else if ( l_AttackType == 9 ) 
		l_Action = WOLF_RUN_ATTACK_STATE;
	else if ( l_AttackType == 10 ) 
		l_Action = WOLF_RUN_ATTACK_STATE;
	else if ( l_AttackType == 11 ) 
		l_Action = WOLF_DEFENSE_STATE;
	else if ( l_AttackType == 12 ) 
		l_Action =  WOLF_JUMP_STATE;
			
	// Más probabilidades de ir al fustrum que no atacar
	else if ( l_AttackType == 13 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 14 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 15 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 16 ) 
		l_Action =  "go_in_to_frustum";

	else if ( l_AttackType == 17 ) 
		l_Action =  WOLF_HOWL_ENEMIES_STATE;
	else if ( l_AttackType == 18 ) 
			l_Action =  WOLF_HOWL_ENEMIES_STATE;
	else if ( l_AttackType == 19 ) 
			l_Action =  WOLF_HOWL_ENEMIES_STATE;
	else if ( l_AttackType == 20 ) 
			l_Action =  WOLF_HOWL_ENEMIES_STATE;

	else if ( l_AttackType == 21 ) 
		l_Action =  WOLF_HOWL_LIFE_STATE;
	else if ( l_AttackType == 22 ) 
			l_Action =  WOLF_HOWL_LIFE_STATE;
	else if ( l_AttackType == 23 ) 
			l_Action =  WOLF_HOWL_LIFE_STATE;
	else if ( l_AttackType == 24 ) 
			l_Action =  WOLF_HOWL_LIFE_STATE;

	return l_Action;
}