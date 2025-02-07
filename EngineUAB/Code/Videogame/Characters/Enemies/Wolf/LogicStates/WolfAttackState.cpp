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
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfDefenseAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlEnemiesAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlLifeAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"

#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"


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
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}

CWolfAttackState::CWolfAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
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

	m_bInPositionToAttack = true;

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
			// print_logger (1, "fatigat despr�s de fer n cops seguits...")
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetTiredState());
		}
		else if ( m_pWolf->GetReceivedHitsXMinut() == m_pWolf->GetTotalReceivedHitsXMinut() ) 
		{
			//LOGGER->AddNewLog(ELL_INFORMATION, "CWolfAttackState::Execute->hits x minut rebuts i per tant bloquejar�...");
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetDefenseState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetDefenseAnimationState());
		}
		else 
		{
			if (!m_bInPositionToAttack)
			{
				float l_Distance = m_pWolf->GetDistanceToPointIn2D(m_SearchedAttackPoint);
				if ( l_Distance <= 3.5f) 
				{
					m_pWolf->GetBehaviors()->SeekOff();
					m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
					m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
					m_bInPositionToAttack = true;
					return;
				} 
				else
				{
					int l_Mask = 1 << ECG_LIMITS;
					if ( m_pWolf->IsPointTouchingGroup(m_pWolf->GetPosition(), l_Mask, 0.5f) ) 
					{
						// Esto nos permite hacer el par�p� un poco. Situarnos delante la c�mara, una simulaci�n de alejarse por cansancio
						m_pWolf->SetToBeTired(false);
						m_pWolf->GetBehaviors()->SeekOff();
						m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
						m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
						m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
						m_bInPositionToAttack = true;
						return;
					}

					m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
					m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
					return;
				}
			}	

			std::string l_ActiveActionState = GetRandomAttackName();
			if ( l_ActiveActionState == WOLF_STILL_ATTACK_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetStillAttackState());
				m_bInPositionToAttack = true;
			}	
			if ( l_ActiveActionState == WOLF_STILL_ATTACK_COMBO_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetStillAttackComboState());
				m_bInPositionToAttack = true;
			}	
			else if ( l_ActiveActionState == WOLF_RUN_ATTACK_STATE ) 
			{
				float l_Distance = m_pWolf->GetDistanceToPlayer();
				if ( l_Distance > m_pWolf->GetProperties()->GetAproximationDistance() / 1.3f) 
				{
					m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetRunAttackState());
				}
				m_bInPositionToAttack = true;
			}	
			else if ( l_ActiveActionState == WOLF_DEFENSE_STATE ) 
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetDefenseState());
				m_bInPositionToAttack = true;
			}
			else if ( l_ActiveActionState == WOLF_HOWL_ENEMIES_STATE ) 
			{
				// Si tenemos m�xima vida y acaba de aparecer el lobo llamamos mis amigos.
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
				m_SearchedAttackPoint = m_pWolf->GetPointInsideCameraFrustum(m_AngleRangeFromCamara);
				m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_SearchedAttackPoint);
				m_pWolf->GetBehaviors()->SeekOn();
				m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
				m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
				LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s fuera del frustum de %f grados", m_Name.c_str(), m_AngleRangeFromCamara);
				m_bInPositionToAttack = false;
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
//	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animaci�n. Tambi�n
// 						puede devolver si solo se mueve sin atacar para enga�ar un poco al player
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
		l_Action =  WOLF_RUN_ATTACK_STATE;
			
	// M�s probabilidades de ir al fustrum que no atacar
	else if ( l_AttackType == 13 ) 
		l_Action =  WOLF_STILL_ATTACK_STATE;
	else if ( l_AttackType == 14 ) 
		l_Action =  WOLF_STILL_ATTACK_COMBO_STATE;
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