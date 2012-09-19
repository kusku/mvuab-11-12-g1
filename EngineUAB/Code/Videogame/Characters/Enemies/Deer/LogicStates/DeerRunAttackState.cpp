#include "DeerRunAttackState.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Math\Vector3.h"
#include "SoundManager.h"
#include "Utils\Timer.h"

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
CDeerRunAttackState::CDeerRunAttackState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CDeerRunAttackState")
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),DEER_RUN_ATTACK_STATE);
}

CDeerRunAttackState::CDeerRunAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),DEER_RUN_ATTACK_STATE);
}


CDeerRunAttackState::~CDeerRunAttackState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerRunAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_pDeer->SetPlayerHasBeenReached( false );
	m_playerPushed = false;

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pDeer->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pDeer->GetSteeringEntity()->GetMass();
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(1.1f);
	//m_pDeer->GetSteeringEntity()->SetMass(0.00500f);

	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_PlayerInitialPosition		= m_pDeer->GetPlayer()->GetSteeringEntity()->GetPosition();
	Vect3f l_Position			= m_pDeer->GetPosition();
	m_InitialDistance			= m_pDeer->GetDistanceToPlayer();
	Vect3f l_RelativePosition	= m_PlayerInitialPosition - m_pDeer->GetPosition();
	Vect3f l_RelativePositionN	= l_RelativePosition.GetNormalized();
	m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 0.f) );

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_FinalAttackPosition);
	m_pDeer->GetBehaviors()->SeparationOn();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOn();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOn();

	m_AnimationDuration = m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_RUN_ATTACK_STATE);

	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerRunAttack"); 

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = DEER_RUN_ATTACK_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif

}

void CDeerRunAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Si el player no estaba ya alcanzado pero ahora se alcanza
		if ( !m_pDeer->GetPlayerHasBeenReached() && m_pDeer->IsPlayerReached() )
		{
			m_pDeer->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Deer_Run_Attack_Charged"); 
		}
			
		// Compruebo si la animación ha finalizado. Trato también el caso que me pase de la posición. Podria ser que la animación tardara demasiado y esto enviaria el deer demasiado lejos
		//float l_CurrentDistance = m_pDeer->GetSteeringEntity()->GetPosition().Distance(m_FinalAttackPosition);
		//float l_PreviousDistance = m_pDeer->GetSteeringEntity()->GetPreviousPosition().Distance(m_FinalAttackPosition);
		//// Compruebo si sigo avanzando, aun no he llegado al punto final
		//if ( (!m_PlayerPositionReached)  )
		//{
		//	if ( l_PreviousDistance < l_CurrentDistance )
		//	{
		//		if ( l_CurrentDistance >= 25.f )
		//			m_PlayerPositionReached = true;
		//	}
		//}

		//if ( m_pAnimationCallback->IsAnimationFinished() || ( m_PlayerPositionReached ) )
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			// Si encontré el player por delante finalizo golpeando
			if ( m_pDeer->GetPlayerHasBeenReached() )
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pDeer->GetID(), m_pDeer->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					//m_pAnimationCallback->Init();
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							LOGGER->AddNewLog(ELL_INFORMATION, "CDeerRunAttackState:Execute->Dispatch" );
						}
					#endif
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CDeerRunAttackState:Execute->El Dispatch es NULL" );
				}
			}
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerRunAttackState::Execute->Golpeo erratico");
					}
				#endif
			}
				
			// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
			m_pDeer->SetHitsDone(m_pDeer->GetHitsDone() + 1);

			// Volvemos al estado anterior
			m_pDeer->GetBehaviors()->SeekOff();
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				
			return;
		}
		// Si no ha finalizado la animación...
		else
		{
			// Si encuentro el player por delante me lo llevo
			if ( m_pDeer->GetPlayerHasBeenReached() )
			{
				if ( !m_playerPushed )
				{
					Vect3f l_Vel = m_pDeer->GetSteeringEntity()->GetVelocity();
					l_Vel.Normalize();
					l_Vel *= m_pDeer->GetProperties()->GetMaxSpeed();
					m_AditionalInfo.Direccion	= l_Vel;
					m_AditionalInfo.ElapsedTime = _ElapsedTime;
				}
					
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pDeer->GetID(), m_pDeer->GetPlayer()->GetID(), Msg_Push, &m_AditionalInfo );
					m_playerPushed = true;
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CDeerRunAttackState:Execute->El Dispatch es NULL" );
				}
			}
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION, "CDeerRunAttackState:Execute->Animation Not finished yet" );
					}
				#endif
			}
		}
			
		m_pDeer->GetBehaviors()->SeekOn();
		m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_PlayerInitialPosition);
				
		// No Rotamos al objetivo y pero si movemos. Esto dará sensación de golpear allí donde estava el target cuando inicie el ataque
		//_CCharacter:face_to( self.target_position, _elapsed_time )
		m_pDeer->FaceTo( m_FinalAttackPosition, _ElapsedTime );
		m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
	}
	// Si l'animación no se ha iniciado
	else
	{
		m_pDeer->GetBehaviors()->SeekOn	();
		m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_PlayerInitialPosition);
		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "CDeerRunAttackState:Execute->Correm al objectiu" );
			}
		#endif
			
		// Ahora azitamos y empezamos la animación de ataque
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAttackAnimationState());
		m_pAnimationCallback->StartAnimation();

		m_pDeer->FaceTo( m_FinalAttackPosition, _ElapsedTime );
		m_pDeer->MoveTo2(  m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

		if ( m_pDeer->GetPlayerHasBeenReached() == false && m_pDeer->IsPlayerReached() )
		{
			m_pDeer->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Deer_Run_Attack_Charged"); 
		}
	}
}


void CDeerRunAttackState::OnExit( CCharacter* _pCharacter )
{
	// Limpiamos animaciones
	/*self.active_animation_name = _CCharacter:get_animation_id("attack_2")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
	self.active_animation_name = _CCharacter:get_animation_id("run")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )*/

	// Quitamos el behaviur
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->SeparationOff();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();

	// Restauramos la velocidad original
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pDeer->GetSteeringEntity()->SetMass(m_OldMass);
	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerExclaim"); 
}

bool CDeerRunAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_Character);
		}

		m_pDeer->RestLife(50); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}