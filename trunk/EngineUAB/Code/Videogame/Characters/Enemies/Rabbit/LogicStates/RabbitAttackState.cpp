#include "RabbitAttackState.h"
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
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "Characters\Character.h"

#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitTiredState.h"
#include "RabbitDefenseState.h"
#include "RabbitStillAttackState.h"
#include "RabbitRunAttackState.h"
#include "RabbitHitState.h"
#include "RabbitIdleState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdle2AnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitDefenseAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitRunAnimationState.h"

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
CRabbitAttackState::CRabbitAttackState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CRabbitAttackState")
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pRabbit		( NULL )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}

CRabbitAttackState::CRabbitAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pRabbit		( NULL )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}


CRabbitAttackState::~CRabbitAttackState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	// Permite saber si el caracter ha realizado los golpes para cansarse. Ponemos a 2 el hits_done para obligar a que cuando entra en este estado haga una minima pausa antes de atacar
	// _CCharacter.hits_done = 0
	m_pRabbit->SetTotalHitsToBeTired(2);
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->SeparationOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
	
	m_bInPositionToAttack = true;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = RABBIT_ATTACK_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif
}

void CRabbitAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( m_pRabbit->IsPlayerAtacable() ) 
	{
		// si venimos del anterior estado haremos una pausa
		if  ( m_pRabbit->IsFatigued() )
		{
			//LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitAttackState::Execute->Is fatigued");
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetTiredState());
		}
		else if ( m_pRabbit->GetReceivedHitsXMinut() == m_pRabbit->GetTotalReceivedHitsXMinut() ) 
		{
			//LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitAttackState::Execute->hits x minut rebuts i per tant bloquejaré...");
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetDefenseState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetDefenseAnimationState());
		}
		else 
		{
			if (!m_bInPositionToAttack)
			{
				float l_Distance = m_pRabbit->GetDistanceToPointIn2D(m_SearchedAttackPoint);
				if ( l_Distance <= 0.5f) 
				{
					m_pRabbit->GetBehaviors()->SeekOff();
					m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
					m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
					m_bInPositionToAttack = true;
					return;
				} 
				else
				{

					int l_Mask = 1 << ECG_LIMITS;
					if ( m_pRabbit->IsPointTouchingGroup(m_pRabbit->GetPosition(), l_Mask, 0.5f) ) 
					{
						// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio
						m_pRabbit->SetToBeTired(false);
						m_pRabbit->GetBehaviors()->SeekOff();
						m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
						m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
						m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
						m_bInPositionToAttack = true;
						return;
					}

					m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
					m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
					return;
				}
			}

			std::string l_ActiveActionState = GetRandomAttackName();
			//LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitAttackState::Execute->Attack Random Sel·leccionat %s per %s", l_ActiveActionState.c_str(), m_pRabbit->GetName().c_str());
				
			if ( l_ActiveActionState == RABBIT_STILL_ATTACK_STATE ) 
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetStillAttackState());
				m_bInPositionToAttack = true;
			}	
			else if ( l_ActiveActionState == RABBIT_RUN_ATTACK_STATE ) 
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetRunAttackState());
				m_bInPositionToAttack = true;
			}	
			else if ( l_ActiveActionState == RABBIT_DEFENSE_STATE ) 
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetDefenseState());
				m_bInPositionToAttack = true;
			}		
			// else if ( l_ActiveActionState == "jump" ) then
				// _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
			else if ( l_ActiveActionState == "go_in_to_frustum" ) 
			{
				m_SearchedAttackPoint = m_pRabbit->GetPointInsideCameraFrustum(m_AngleRangeFromCamara);
				m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_SearchedAttackPoint);
				m_pRabbit->GetBehaviors()->SeekOn();
				m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
				m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
				LOGGER->AddNewLog(ELL_INFORMATION, "CCharacter::GoIntoCameraFrustum-> %s fuera del frustum de %f grados", m_Name.c_str(), m_AngleRangeFromCamara);
				/*m_pRabbit->GoIntoCameraFrustum(l_Angle, _ElapsedTime);*/
				m_bInPositionToAttack = false;
			}

			#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s in some attack...", m_pRabbit->GetName().c_str() );
			}
			#endif

		} 	// End fatigue
	}	
	// Si el player NO es atacable lo volvemos a preparar o a perseguir
	else 
	{
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetPreparedToAttack());
	}
}

void CRabbitAttackState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->SeparationOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
}

bool CRabbitAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	return m_pRabbit->CallHitState(m_pRabbit, _Telegram);
}

// --------------------------------------------------------------------------------------------------------
//	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animación. También
// 						puede devolver si solo se mueve sin atacar para engañar un poco al player
// --------------------------------------------------------------------------------------------------------
std::string CRabbitAttackState::GetRandomAttackName(void)
{
	std::string l_Action;	

	int l_AttackType = BoostRandomHelper::GetInt(1,14);
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
		l_Action =  RABBIT_STILL_ATTACK_STATE;
			
	// Más probabilidades de ir al fustrum que no atacar
	else if ( l_AttackType == 9 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 10 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 11 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 12 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 13 ) 
		l_Action =  "go_in_to_frustum";
	else if ( l_AttackType == 14 ) 
		l_Action =  "go_in_to_frustum";

	return l_Action;
}