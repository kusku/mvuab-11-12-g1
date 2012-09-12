#include "RabbitRunAttackState.h"
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
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitAttackState.h"
#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitHitState.h"

#include "RabbitHitAnimationState.h"
#include "RabbitIdleAnimationState.h"
#include "RabbitRunAttackAnimationState.h"
#include "RabbitRunAnimationState.h"

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
CRabbitRunAttackState::CRabbitRunAttackState( void )
	: CState				("CRabbitRunAttackState")
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RABBIT_RUN_ATTACK_STATE);
}

CRabbitRunAttackState::CRabbitRunAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RABBIT_RUN_ATTACK_STATE);
}


CRabbitRunAttackState::~CRabbitRunAttackState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRunAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pRabbit->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pRabbit->GetSteeringEntity()->GetMass();
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(0.13f);
	m_pRabbit->GetSteeringEntity()->SetMass(0.00500f);

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();

	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_CurrentDistance = m_pRabbit->GetDistanceToPlayer();

	m_pAnimationCallback->Init();

	m_CurrentDuration = 0;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Enter Run Attack");
		}
	#endif

}

void CRabbitRunAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	m_CurrentDuration += _ElapsedTime;

	// Si és atacable hago cualquier gestión
	if ( m_pRabbit->IsPlayerAtacable() )
	{
		if ( m_pAnimationCallback->IsAnimationStarted() ) 
		{
			// Compruebo si la animación ha finalizado
			if ( m_pAnimationCallback->IsAnimationFinished() && m_pRabbit->IsPlayerInsideImpactDistance() ) 
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pRabbit->GetID(), m_pRabbit->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					//m_pAnimationCallback->Init();
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CRabbitRunAttackState:Execute->El Dispatch es NULL" );
				}
					
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pRabbit->SetHitsDone(m_pRabbit->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pRabbit->GetBehaviors()->SeekOff();
				m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
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
				m_pRabbit->GetBehaviors()->SeekOff();

				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pRabbit->SetHitsDone(m_pRabbit->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pRabbit->GetLogicFSM()->RevertToPreviousState();
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());

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
				float l_Distance = m_pRabbit->GetDistanceToPlayer();
		
				// Pararemos de correr si estamos en el momento de impacto o que la distancia al player no sea mayor que la inicial ya que indicará 
				// que el ataque seguramente falló y así evitamos que exista un pequeño retroceso de volver hacia el player
				if ( m_pRabbit->IsPlayerInsideImpactDistance() ) //|| ( l_Distance > m_CurrentDistance ) ) 
				{
					m_pRabbit->GetBehaviors()->SeekOff();
					m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Seek Off");
						}
					#endif
				}
				else
				{
					m_AnimationDuration = m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_RUN_ATTACK_STATE);
					//int l_AnimationTracks = m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_RUN_ATTACK_STATE);

					if ( m_CurrentDuration >= m_AnimationDuration )
					{
						m_pRabbit->GetBehaviors()->SeekOn();
						m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
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
				m_pRabbit->FaceTo( m_pRabbit->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}	
		}
		// Si l'animación no se ha iniciado
		else
		{
			// Caso que ha corrido y estoy dentro de la distancia de impacto-->Azitamos!
			if ( m_pRabbit->IsPlayerInsideImpactDistance() ) 
			{
				m_pRabbit->GetBehaviors()->SeekOff();
				m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Atac sense corre!!");
					}
				#endif
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAttackAnimationState());
				m_pAnimationCallback->StartAnimation();
			}
			else
			{
				// Primer estado que se ejecutará. Si está lejos nos acercamos con gran velocidad Corremos rápido hacía el player. 
				// Para ello hay que saber el tiempo de la animación y cuando hay que activarla. 

				// Pillo la duración de la animación
				m_AnimationDuration = m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_RUN_ATTACK_STATE);

				// Distancia recorrida según la duración de la animación a una velocidad determinada máxima
				//float l_DistanciaRecorridaAnimacion = m_pRabbit->GetSteeringEntity()->GetMaxSpeed() * m_AnimationDuration;
				float l_DistanciaRecorridaAnimacion = m_pRabbit->GetSteeringEntity()->GetMaxSpeed() * m_AnimationDuration * _ElapsedTime;
				//float l_DistanciaRecorridaAnimacion = m_pRabbit->GetSteeringEntity()->GetMaxSpeed() * m_AnimationDuration + 1/2*m_pRabbit->GetSteeringEntity()->GetMaxAcceleration()*m_AnimationDuration*m_AnimationDuration * 1000;
				float l_DistanceToPlayer = m_pRabbit->GetDistanceToPlayer();

				// En este caso empezamos a ejecutar la animación
				if ( l_DistanceToPlayer <= l_DistanciaRecorridaAnimacion ) 
				{
					m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAttackAnimationState());
					m_pAnimationCallback->StartAnimation();
				}

				m_pRabbit->GetBehaviors()->SeekOn();
				m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());

				
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Correm a sac!!");
					}
				#endif

				// _CCharacter.behaviors:pursuit_on()
				
			}

			m_pRabbit->FaceTo( m_pRabbit->GetSteeringEntity()->GetPosition(), _ElapsedTime );
			m_pRabbit->MoveTo2(  m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
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


void CRabbitRunAttackState::OnExit( CCharacter* _Character )
{
	// Limpiamos animaciones
	/*self.active_animation_name = _CCharacter:get_animation_id("attack_2")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
	self.active_animation_name = _CCharacter:get_animation_id("run")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )*/

	// Quitamos el behaviur
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
				
	// Restauramos la velocidad original
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pRabbit->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CRabbitRunAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
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