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
#include "Characters\Enemies\Deer\AnimationStates\DeerRunAnimationState.h"

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
CDeerAttackState::CDeerAttackState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CDeerAttackState")
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pDeer		( NULL )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}

CDeerAttackState::CDeerAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pDeer		( NULL )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
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

	m_bInPositionToAttack = true;

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
			if (!m_bInPositionToAttack)
			{
				float l_Distance = m_pDeer->GetPosition().Distance(m_SearchedAttackPoint);
				if ( l_Distance <= 0.5f) 
				{
					m_pDeer->GetBehaviors()->SeekOff();
					m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
					m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
					m_bInPositionToAttack = true;
					return;
				} 
				else
				{
					m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
					return;
				}

				//m_pDeer->GoIntoCameraFrustum(l_Angle, _ElapsedTime);
				//return;
			}	
			
			std::string l_ActiveActionState = GetRandomAttackName();
			LOGGER->AddNewLog(ELL_INFORMATION, "CDeerAttackState::Execute->Attack Random Sel·leccionat %s", l_ActiveActionState.c_str());
				
			if ( l_ActiveActionState == DEER_STILL_ATTACK_STATE ) 
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetStillAttackState());
				m_bInPositionToAttack = true;
			}	
			else if ( l_ActiveActionState == DEER_RUN_ATTACK_STATE ) 
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetRunAttackState());
				m_bInPositionToAttack = true;
			}	
			else if ( l_ActiveActionState == DEER_DEFENSE_STATE ) 
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetDefenseState());
				m_bInPositionToAttack = true;
			}	
			// else if ( l_ActiveActionState == "jump" ) then
				// _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
			else if ( l_ActiveActionState == "go_in_to_frustum" ) 
			{
				m_SearchedAttackPoint = m_pDeer->GetPointInsideCameraFrustum(m_AngleRangeFromCamara);
				m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_SearchedAttackPoint);
				m_pDeer->GetBehaviors()->SeekOn();
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
				m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
				LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s fuera del frustum de %f grados", m_Name.c_str(), m_AngleRangeFromCamara);
				m_bInPositionToAttack = false;
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

bool CDeerAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	return m_pDeer->CallHitState(m_pDeer, _Telegram);
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
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 10 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 11 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 12 ) 
		l_Action =  "go_in_to_frustum";

	return l_Action;
}