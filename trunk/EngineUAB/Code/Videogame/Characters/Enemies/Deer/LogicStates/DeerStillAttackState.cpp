#include "DeerStillAttackState.h"
#include "GameProcess.h"
#include "SoundManager.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\Telegram.h"

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"

#include "DeerPursuitState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerHitState.h"
#include "DeerAttackState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerStillAttackAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerRunAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"

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
CDeerStillAttackState::CDeerStillAttackState( void )
	: CState				("CDeerStillAttackState")
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_PlayerReached		( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_STILL_ATTACK_STATE);
}

CDeerStillAttackState::CDeerStillAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_PlayerReached		( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_STILL_ATTACK_STATE);
}


CDeerStillAttackState::~CDeerStillAttackState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerStillAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Enter Still Attack");
	}
#endif

	m_PlayerReached = false;
	m_pAnimationCallback->Init();
}

void CDeerStillAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
	if ( m_pDeer->IsPlayerAtacable() )
	{
		if ( m_pAnimationCallback->IsAnimationStarted() ) 
		{
			if ( m_PlayerReached == false && m_pDeer->IsPlayerReached() )
			{
				m_PlayerReached = true;
			}

			CORE->GetSoundManager()->PlayEvent("Play_EFX_Punch2");
			CORE->GetSoundManager()->PlayEvent("Play_EFX_Punch3");

			// Compruebo si la animación a finalizado
			if ( m_pAnimationCallback->IsAnimationFinished() && m_PlayerReached )
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pDeer->GetID(), m_pDeer->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Envio mensaje de tocado");
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CDeerStillAttackState:Execute->El Dispatch es NULL" );
				}
					
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pDeer->SetHitsDone(m_pDeer->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pDeer->GetLogicFSM()->RevertToPreviousState();
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Dispatch");
					}
				#endif

				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else if ( m_pAnimationCallback->IsAnimationFinished() && !m_PlayerReached )
			{
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pDeer->SetHitsDone(m_pDeer->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pDeer->GetLogicFSM()->RevertToPreviousState();
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Still Attack fallit ");
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Golpeo erratico");

					}
				#endif

				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

				return;
			}
			// NO!! Esto hace que luego corra mientras ataca y queda de pena. Mantengo el comentario para recordarlo.
			// En caso que no estemos en una distancia de ataque nos salimos. Este se comenta pq se prevee que falle y corte la animacion
			/*else if ( !m_pDeer->IsPlayerInsideImpactDistance() )
			{
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Allunyat!");
					}
				#endif

				return;
			}*/
			// En otro caso actualizamos el tiempo de animacion que aun no finalizó la animación
			else 
			{
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("NOT FINISHED YET!");
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Animacion en curso");
					}
				#endif

				// hay que cancelar la animación
				/*if ( !m_pDeer->IsPlayerInsideImpactDistance() )
				{
					m_pDeer->GetLogicFSM()->RevertToPreviousState();
					m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				}*/
			}
		}
		else
		{
			if ( m_pDeer->IsPlayerInsideImpactDistance() ) 
			{
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
				/*self.active_animation_id = _CCharacter:get_animation_id("run")
				_CCharacter:get_animation_model():clear_cycle( self.active_animation_id, 0.3 )
					
				self.active_animation_id = _CCharacter:get_animation_id("attack_1")
				_CCharacter:get_animation_model():execute_action( self.active_animation_id, 0.3 )*/
				
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetStillAttackAnimationState());
				m_pAnimationCallback->StartAnimation();

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Inici atac");
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Inicio Animacion");
					}
				#endif
			}
			else 
			{
				// Nos acercamos 
				// _CCharacter.behaviors:pursuit_on()
				m_pDeer->GetBehaviors()->SeekOn();
				m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
				
				// Rotamos al objetivo y movemos
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Ens apropem primer");
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Nos acercamos primero");
					}
				#endif
			}
		}
	}
	else
	{
		// nos volvemos
		/*m_pDeer->GetLogicFSM()->RevertToPreviousState();
		m_pDeer->GetGraphicFSM()->RevertToPreviousState();*/
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	} 
}


void CDeerStillAttackState::OnExit( CCharacter* _Character )
{
	// nos volvemos
	/*m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());*/
	LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Salgo!");

}

bool CDeerStillAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_Character);
		}

		m_pDeer->RestLife(10); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		return true;
	}

	return false;
}

