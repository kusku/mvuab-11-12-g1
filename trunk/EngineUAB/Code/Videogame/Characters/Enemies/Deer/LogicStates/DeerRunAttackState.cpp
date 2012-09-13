#include "DeerRunAttackState.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Math\Vector3.h"
#include "SoundManager.h"
#include "Math\Matrix44.h"

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
	, m_PlayerReached		( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_RUN_ATTACK_STATE);
}

CDeerRunAttackState::CDeerRunAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_PlayerReached		( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_RUN_ATTACK_STATE);
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

	m_PlayerReached			= false;
	m_PlayerPositionReached = false;
	m_playerPushed			= false;

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
	m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 5.f));

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_FinalAttackPosition);
	
	m_AnimationDuration = m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_RUN_ATTACK_STATE);
	m_CurrentDuration = 0;

	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent("Play_DeerRunAttack"); 

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Run Attack");
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
			if ( m_PlayerReached == false && m_pDeer->IsPlayerReached() )
			{
				m_PlayerReached = true;
				CORE->GetSoundManager()->PlayEvent("Play_DeerRunAttackCharged"); 
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
				if ( m_PlayerReached )
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
						LOGGER->AddNewLog(ELL_INFORMATION, "CDeerRunAttackState:Execute->Dispatch" );
					}
				#endif
				
				return;
			}
			// Si no ha finalizado la animación...
			else
			{
				// Si encuentro el player por delante me lo llevo
				if ( m_PlayerReached )
				{
					if ( !m_playerPushed )
					{
						m_AditionalInfo.Direccion	= m_pDeer->GetSteeringEntity()->GetVelocity();
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

			if ( m_PlayerReached == false && m_pDeer->IsPlayerReached() )
			{
				m_PlayerReached = true;
				CORE->GetSoundManager()->PlayEvent("Play_DeerRunAttackCharged"); 
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

		m_pDeer->RestLife(20); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		return true;
	}

	return false;
}