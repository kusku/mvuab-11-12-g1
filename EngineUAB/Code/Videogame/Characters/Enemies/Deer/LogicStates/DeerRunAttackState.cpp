#include "DeerRunAttackState.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Math\Vector3.h"

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

#include "DeerAttackState.h"
#include "DeerPursuitState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerHitState.h"
#include "DeerRunAttackState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerRunAttackAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerRunAnimationState.h"

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
CDeerRunAttackState::CDeerRunAttackState( void )
	: CState				("CDeerRunAttackState")
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RUN_ATTACK_STATE);
}

CDeerRunAttackState::CDeerRunAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RUN_ATTACK_STATE);
}


CDeerRunAttackState::~CDeerRunAttackState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerRunAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pDeer->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pDeer->GetSteeringEntity()->GetMass();
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(0.13f);
	m_pDeer->GetSteeringEntity()->SetMass(0.00500f);

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
		
	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_CurrentDistance = m_pDeer->GetDistanceToPlayer();

	m_pAnimationCallback->Init();

	m_CurrentDuration = 0;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Enter Run Attack");
		}
	#endif

}

void CDeerRunAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	m_CurrentDuration += _ElapsedTime;

	// Si és atacable hago cualquier gestión
	if ( m_pDeer->IsPlayerAtacable() )
	{
		if ( m_pAnimationCallback->IsAnimationStarted() ) 
		{
			// Compruebo si la animación ha finalizado
			if ( m_pAnimationCallback->IsAnimationFinished() && m_pDeer->IsPlayerInsideImpactDistance() ) 
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pDeer->GetID(), m_pDeer->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					//m_pAnimationCallback->Init();
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CDeerRunAttackState:Execute->El Dispatch es NULL" );
				}
					
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pDeer->SetHitsDone(m_pDeer->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Dispatch");
					}
				#endif
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else if ( m_pAnimationCallback->IsAnimationFinished() && !m_pDeer->IsPlayerInsideImpactDistance() )
			{
				m_pDeer->GetBehaviors()->SeekOff();

				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pDeer->SetHitsDone(m_pDeer->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pDeer->GetLogicFSM()->RevertToPreviousState();
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Run Attack fallit ");
					}
				#endif

				return;
			}
			else 
			{
				float l_Distance = m_pDeer->GetDistanceToPlayer();
		
				// Pararemos de correr si estamos en el momento de impacto o que la distancia al player no sea mayor que la inicial ya que indicará 
				// que el ataque seguramente falló y así evitamos que exista un pequeño retroceso de volver hacia el player
				if ( m_pDeer->IsPlayerInsideImpactDistance() || ( l_Distance > m_CurrentDistance ) ) 
				{
					m_pDeer->GetBehaviors()->SeekOff();
					m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Seek Off");
						}
					#endif
				}
				else
				{
					m_AnimationDuration = m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(RUN_ATTACK_STATE) / 3;

					if ( m_CurrentDuration >= m_AnimationDuration )
					{
						m_pDeer->GetBehaviors()->SeekOn();
						m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
					}
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("NOT FINISHED YET!");
						}
					#endif
				}
			
				// No Rotamos al objetivo y pero si movemos. Esto dará sensación de golpear allí donde estava el target cuando inicie el ataque
				//_CCharacter:face_to( self.target_position, _elapsed_time )
				m_pDeer->FaceTo( m_pDeer->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}	
		}
		// Si l'animación no se ha iniciado
		else
		{
			// Caso que ha corrido y estoy dentro de la distancia de impacto-->Azitamos!
			if ( m_pDeer->IsPlayerInsideImpactDistance() ) 
			{
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Atac sense corre!!");
					}
				#endif
			}
			else
			{
				// Primer estado que se ejecutará. Si está lejos nos acercamos con gran velocidad
				// Corremos rápido hacía el player
				// _CCharacter.behaviors:pursuit_on()
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Correm a sac!!");
					}
				#endif
			}

			// Ahora azitamos y empezamos la animación de ataque
			if ( m_pDeer != NULL ) 
			{
				/*int l_Num = m_pDeer->GetAnimationID("attack_1");
				m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
				
				l_Num = m_pDeer->GetAnimationID("run");
				m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
					
				l_Num = m_pDeer->GetAnimationID("attack_2");
				m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );*/

				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAttackAnimationState());
				m_pAnimationCallback->StartAnimation();

				m_pDeer->FaceTo( m_pDeer->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2(  m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}
			else 
			{
				LOGGER->AddNewLog(ELL_ERROR, "CDeerStillAttackState:Execute->El Character Deer es NULL" );
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


void CDeerRunAttackState::OnExit( CCharacter* _Character )
{
	// Limpiamos animaciones
	/*self.active_animation_name = _CCharacter:get_animation_id("attack_2")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
	self.active_animation_name = _CCharacter:get_animation_id("run")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )*/

	// Quitamos el behaviur
	m_pDeer->GetBehaviors()->SeekOff();
					
	// Restauramos la velocidad original
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pDeer->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CDeerRunAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}