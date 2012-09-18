#include "WolfStillAttackState.h"
#include "GameProcess.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\Telegram.h"

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfHitState.h"
#include "WolfAttackState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfStillAttackAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfStillAttackState::CWolfStillAttackState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CWolfStillAttackState")
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_STILL_ATTACK_STATE);
}

CWolfStillAttackState::CWolfStillAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_STILL_ATTACK_STATE);
}


CWolfStillAttackState::~CWolfStillAttackState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfStillAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = "Enter Still Attack";
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
	}
#endif

	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();

	m_pAnimationCallback->Init();
}

void CWolfStillAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}
	
	// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
	if ( m_pWolf->IsPlayerAtacable() )
	{
		if ( m_pAnimationCallback->IsAnimationStarted() ) 
		{
			// Compruebo si la animación a finalizado
			if ( m_pAnimationCallback->IsAnimationFinished() ) 
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CWolfStillAttackState:Execute->El Dispatch es NULL" );
				}
					
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pWolf->GetLogicFSM()->RevertToPreviousState();
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "Dispatch";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State);
					}
				#endif
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else if ( m_pAnimationCallback->IsAnimationFinished() && !m_pWolf->IsPlayerInsideImpactDistance() )
			{
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

				// Volvemos al estado anterior
				//m_pWolf->GetLogicFSM()->RevertToPreviousState();
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "Still Attack fallit";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif

				return;
			}
			// NO!! Esto hace que luego corra mientras ataca y queda de pena. Mantengo el comentario para recordarlo.
			// En caso que no estemos en una distancia de ataque nos salimos. Este se comenta pq se prevee que falle y corte la animacion
			/*else if ( !m_pWolf->IsPlayerInsideImpactDistance() )
			{
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), "Allunyat!");
					}
				#endif

				return;
			}*/
			// En otro caso actualizamos el tiempo de animacion
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "NOT FINISHED YET!";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif
			}
		}
		else
		{
			if ( m_pWolf->IsPlayerInsideImpactDistance() ) 
			{
				m_pWolf->GetBehaviors()->SeekOff();
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
				/*self.active_animation_id = _CCharacter:get_animation_id("run")
				_CCharacter:get_animation_model():clear_cycle( self.active_animation_id, 0.3 )
					
				self.active_animation_id = _CCharacter:get_animation_id("attack_1")
				_CCharacter:get_animation_model():execute_action( self.active_animation_id, 0.3 )*/
				
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetStillAttackAnimationState());
				m_pAnimationCallback->StartAnimation();
			}
			else 
			{
				// Nos acercamos 
				// _CCharacter.behaviors:pursuit_on()
				m_pWolf->GetBehaviors()->SeekOn();
				m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
				if ( m_pWolf != NULL ) 
				{
					// self.active_animation_name = _CCharacter:get_animation_id("attack_1")
					// _CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
						
					/*self.active_animation_name = _CCharacter:get_animation_id("run")
					_CCharacter:get_animation_model():blend_cycle( self.active_animation_name, 0.3 )*/
					m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
				}
				else 
				{
					LOGGER->AddNewLog(ELL_ERROR, "CWolfStillAttackState:Execute->El Character Rabbit es NULL" );
				}
				
				// Rotamos al objetivo y movemos
				m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "Ens apropem primer";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif
			}
		}
	}
	else
	{
		// nos volvemos
		/*m_pWolf->GetLogicFSM()->RevertToPreviousState();
		m_pWolf->GetGraphicFSM()->RevertToPreviousState();*/
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	} 
}


void CWolfStillAttackState::OnExit( CCharacter* _pCharacter )
{
	// nos volvemos
	/*m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());*/
}

bool CWolfStillAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
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

