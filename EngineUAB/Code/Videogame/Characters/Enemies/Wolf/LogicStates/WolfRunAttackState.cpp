#include "WolfRunAttackState.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "StatesMachine\MessageDispatcher.h"
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
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfAttackState.h"
#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAttackAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"

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
CWolfRunAttackState::CWolfRunAttackState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CWolfRunAttackState")
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_RUN_ATTACK_STATE);
}

CWolfRunAttackState::CWolfRunAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_RUN_ATTACK_STATE);
}


CWolfRunAttackState::~CWolfRunAttackState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfRunAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pWolf->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pWolf->GetSteeringEntity()->GetMass();
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(0.13f);
	m_pWolf->GetSteeringEntity()->SetMass(0.00500f);

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pWolf->GetBehaviors()->SeekOn();
	m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
		
	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_CurrentDistance = m_pWolf->GetDistanceToPlayer();

	m_pAnimationCallback->Init();

	m_CurrentDuration = 0;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Enter Run Attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfRunAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_CurrentDuration += _ElapsedTime;

	// Si és atacable hago cualquier gestión
	if ( m_pWolf->IsPlayerAtacable() )
	{
		if ( m_pAnimationCallback->IsAnimationStarted() ) 
		{
			// Compruebo si la animación ha finalizado
			if ( m_pAnimationCallback->IsAnimationFinished() && m_pWolf->IsPlayerInsideImpactDistance() ) 
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					//m_pAnimationCallback->Init();
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CWolfRunAttackState:Execute->El Dispatch es NULL" );
				}
					
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pWolf->GetBehaviors()->SeekOff();
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "Dispatch";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else if ( m_pAnimationCallback->IsAnimationFinished() && !m_pWolf->IsPlayerInsideImpactDistance() )
			{
				m_pWolf->GetBehaviors()->SeekOff();

				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pWolf->GetLogicFSM()->RevertToPreviousState();
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "Run Attack fallit ";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif

				return;
			}
			else 
			{
				float l_Distance = m_pWolf->GetDistanceToPlayer();
		
				// Pararemos de correr si estamos en el momento de impacto o que la distancia al player no sea mayor que la inicial ya que indicará 
				// que el ataque seguramente falló y así evitamos que exista un pequeño retroceso de volver hacia el player
				if ( m_pWolf->IsPlayerInsideImpactDistance() || ( l_Distance > m_CurrentDistance ) ) 
				{
					m_pWolf->GetBehaviors()->SeekOff();
					m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							std::string l_State = "Seek off";
							CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
						}
					#endif
				}
				else
				{
					m_AnimationDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_RUN_ATTACK_STATE) / 3;

					if ( m_CurrentDuration >= m_AnimationDuration )
					{
						m_pWolf->GetBehaviors()->SeekOn();
						m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
					}
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							std::string l_State = "NOT FINISHED YET!";
							CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
						}
					#endif
				}
			
				// No Rotamos al objetivo y pero si movemos. Esto dará sensación de golpear allí donde estava el target cuando inicie el ataque
				//_CCharacter:face_to( self.target_position, _elapsed_time )
				m_pWolf->FaceTo( m_pWolf->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}	
		}
		// Si l'animación no se ha iniciado
		else
		{
			// Caso que ha corrido y estoy dentro de la distancia de impacto-->Azitamos!
			if ( m_pWolf->IsPlayerInsideImpactDistance() ) 
			{
				m_pWolf->GetBehaviors()->SeekOff();
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{							
						std::string l_State = "Atac sense corre!!!";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif
			}
			else
			{
				// Primer estado que se ejecutará. Si está lejos nos acercamos con gran velocidad
				// Corremos rápido hacía el player
				// _CCharacter.behaviors:pursuit_on()
				m_pWolf->GetBehaviors()->SeekOff();
				m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = "Correm a sac!!";
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif
			}

			// Ahora azitamos y empezamos la animación de ataque
			if ( m_pWolf != NULL ) 
			{
				/*int l_Num = m_pWolf->GetAnimationID("attack_1");
				m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
				
				l_Num = m_pWolf->GetAnimationID("run");
				m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
					
				l_Num = m_pWolf->GetAnimationID("attack_2");
				m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );*/

				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAttackAnimationState());
				m_pAnimationCallback->StartAnimation();

				m_pWolf->FaceTo( m_pWolf->GetSteeringEntity()->GetPosition(), _ElapsedTime );
				m_pWolf->MoveTo2(  m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}
			else 
			{
				LOGGER->AddNewLog(ELL_ERROR, "CRabbitStillAttackState:Execute->El Character Rabbit es NULL" );
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


void CWolfRunAttackState::OnExit( CCharacter* _pCharacter )
{
	// Limpiamos animaciones
	/*self.active_animation_name = _CCharacter:get_animation_id("attack_2")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
	self.active_animation_name = _CCharacter:get_animation_id("run")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )*/

	// Quitamos el behaviur
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();
				
	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pWolf->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CWolfRunAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(1000); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		return true;
	}

	return false;
}