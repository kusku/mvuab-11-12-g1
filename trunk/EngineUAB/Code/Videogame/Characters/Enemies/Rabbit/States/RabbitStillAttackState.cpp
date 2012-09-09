#include "RabbitStillAttackState.h"
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
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitHitState.h"
#include "RabbitAttackState.h"

#include "RabbitHitAnimationState.h"
#include "RabbitStillAttackAnimationState.h"
#include "RabbitRunAnimationState.h"
#include "RabbitIdleAnimationState.h"

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
CRabbitStillAttackState::CRabbitStillAttackState( void )
	: CState				("CRabbitStillAttackState")
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RABBIT_STILL_ATTACK_STATE);
}

CRabbitStillAttackState::CRabbitStillAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RABBIT_STILL_ATTACK_STATE);
}


CRabbitStillAttackState::~CRabbitStillAttackState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitStillAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Enter Still Attack");
	}
#endif

	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();

	m_pAnimationCallback->Init();
}

void CRabbitStillAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}
	
	// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
	if ( m_pRabbit->IsPlayerAtacable() )
	{
		if ( m_pAnimationCallback->IsAnimationStarted() ) 
		{
			// Compruebo si la animación a finalizado
			if ( m_pAnimationCallback->IsAnimationFinished() && m_pRabbit->IsPlayerInsideImpactDistance() )
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pRabbit->GetID(), m_pRabbit->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CRabbitStillAttackState:Execute->El Dispatch es NULL" );
				}
					
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pRabbit->SetHitsDone(m_pRabbit->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pRabbit->GetLogicFSM()->RevertToPreviousState();
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Dispatch");
					}
				#endif
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else if ( m_pAnimationCallback->IsAnimationFinished() && !m_pRabbit->IsPlayerInsideImpactDistance() )
			{
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pRabbit->SetHitsDone(m_pRabbit->GetHitsDone() + 1);

				// Volvemos al estado anterior
				//m_pRabbit->GetLogicFSM()->RevertToPreviousState();
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Still Attack fallit ");
					}
				#endif

				return;
			}
			// NO!! Esto hace que luego corra mientras ataca y queda de pena. Mantengo el comentario para recordarlo.
			// En caso que no estemos en una distancia de ataque nos salimos. Este se comenta pq se prevee que falle y corte la animacion
			/*else if ( !m_pRabbit->IsPlayerInsideImpactDistance() )
			{
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Allunyat!");
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
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("NOT FINISHED YET!");
					}
				#endif
			}
		}
		else
		{
			if ( m_pRabbit->IsPlayerInsideImpactDistance() ) 
			{
				m_pRabbit->GetBehaviors()->SeekOff();
				m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pRabbit->FaceTo( m_pRabbit->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
				/*self.active_animation_id = _CCharacter:get_animation_id("run")
				_CCharacter:get_animation_model():clear_cycle( self.active_animation_id, 0.3 )
					
				self.active_animation_id = _CCharacter:get_animation_id("attack_1")
				_CCharacter:get_animation_model():execute_action( self.active_animation_id, 0.3 )*/
				
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetStillAttackAnimationState());
				m_pAnimationCallback->StartAnimation();
			}
			else 
			{
				// Nos acercamos 
				// _CCharacter.behaviors:pursuit_on()
				m_pRabbit->GetBehaviors()->SeekOn();
				m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
				if ( m_pRabbit != NULL ) 
				{
					// self.active_animation_name = _CCharacter:get_animation_id("attack_1")
					// _CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
						
					/*self.active_animation_name = _CCharacter:get_animation_id("run")
					_CCharacter:get_animation_model():blend_cycle( self.active_animation_name, 0.3 )*/
					m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
				}
				else 
				{
					LOGGER->AddNewLog(ELL_ERROR, "CRabbitStillAttackState:Execute->El Character Rabbit es NULL" );
				}
				
				// Rotamos al objetivo y movemos
				m_pRabbit->FaceTo( m_pRabbit->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Ens apropem primer");
					}
				#endif
			}
		}
	}
	else
	{
		// nos volvemos
		/*m_pRabbit->GetLogicFSM()->RevertToPreviousState();
		m_pRabbit->GetGraphicFSM()->RevertToPreviousState();*/
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	} 
}


void CRabbitStillAttackState::OnExit( CCharacter* _Character )
{
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();

}

bool CRabbitStillAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_Character);
		}

		m_pRabbit->RestLife(1000); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		return true;
	}

	return false;
}

