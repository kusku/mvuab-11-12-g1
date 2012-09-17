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
#include "DeerIdleState.h"

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

#include "Particles\ParticleEmitter.h"
#include "Particles\ParticleEmitterManager.h"

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
	, m_pActionStateCallback( 0, 1 )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_STILL_ATTACK_STATE);
	m_pParticleEmitter1	 = CORE->GetParticleEmitterManager()->GetResource("RightHand1");
	m_pParticleEmitter2	 = CORE->GetParticleEmitterManager()->GetResource("RightHand11");
	m_pParticleEmitter3	 = CORE->GetParticleEmitterManager()->GetResource("LeftHand1");
	m_pParticleEmitter4	 = CORE->GetParticleEmitterManager()->GetResource("LeftHand11");
}

CDeerStillAttackState::CDeerStillAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_pActionStateCallback( 0, 1 )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_STILL_ATTACK_STATE);
	m_pParticleEmitter1	 = CORE->GetParticleEmitterManager()->GetResource("RightHand1");
	m_pParticleEmitter2	 = CORE->GetParticleEmitterManager()->GetResource("RightHand11");
	m_pParticleEmitter3	 = CORE->GetParticleEmitterManager()->GetResource("LeftHand1");
	m_pParticleEmitter4	 = CORE->GetParticleEmitterManager()->GetResource("LeftHand11");
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
		std::string l_State = DEER_STILL_ATTACK_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
	}
#endif

	//Lanza el sistema de partículas
	SetParticlePosition(m_pDeer);
	
	// Gestión de sonidos e impacto
	m_FirstHitReached	= false;
	m_SecondHitReached	= false;
	m_FirstHitDone		= false;
	m_SecondHitDone		= false;
	m_SoundPlayed1		= false;
	m_SoundPlayed2		= false;
	m_pAnimationCallback->Init();
	m_pDeer->SetPlayerHasBeenReached( false );
	
	//CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerExclaim"); 
	
	m_pActionStateCallback.InitAction(0, m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE) );
}

void CDeerStillAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	SetParticlePosition(m_pDeer);

	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{	
			// Aseguro que se tocó o no según la variable que gestiona esto en los golpeos
			m_pDeer->SetPlayerHasBeenReached(m_FirstHitReached||m_SecondHitReached );

			if ( m_pDeer->GetPlayerHasBeenReached() )
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

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog( ELL_INFORMATION, "CDeerStillAttackState:Execute->Dispatch" );
					}
				#endif

				// Volvemos a idle
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
				m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else 
			{
				// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
				m_pDeer->SetHitsDone(m_pDeer->GetHitsDone() + 1);

				// Volvemos al estado anterior
				m_pDeer->GetLogicFSM()->RevertToPreviousState();
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());

				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
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
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), "Allunyat!");
					}
				#endif

				return;
			}*/
		}
		// En otro caso actualizamos el tiempo de animacion que aun no finalizó la animación
		else 
		{
			m_pDeer->GetBehaviors()->SeekOff();
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			//float t = m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE);

			if ( m_pActionStateCallback.IsActionInTime( 0.2f ) && !m_FirstHitDone )
			{
				m_pParticleEmitter1->EjectParticles();
				m_pParticleEmitter2->EjectParticles();

				m_FirstHitDone = true;		// Ahora ya no entraremos en este condicional

				if ( m_pDeer->IsPlayerReached() )
				{
					m_FirstHitReached = true;
				}

				// Sonido de la 1a bofetada 
				if ( m_FirstHitReached && !m_SoundPlayed1 )
				{
					m_SoundPlayed1 = true;
					CORE->GetSoundManager()->PlayEvent("Play_EFX_Punch3"); 
				}
				// Sonido de la 1a bofetada fallida
				else if ( !m_FirstHitReached && !m_SoundPlayed1 )
				{
					CORE->GetSoundManager()->PlayEvent("Play_EFX_Slap1"); 
					m_SoundPlayed1 = true;
				}
			}

			if ( m_pActionStateCallback.IsActionInTime( 0.7f ) && !m_SecondHitDone )
			{
				m_pParticleEmitter3->EjectParticles();
				m_pParticleEmitter4->EjectParticles();


				m_SecondHitDone = true;		// Esto permite ver si ya se hizo el hit y comprobar solo una sola vez y justo en el momento del impacto si se alcanzó el player

				if ( m_pDeer->IsPlayerReached() )
				{
					m_SecondHitReached = true; 
				}

				// Sonido de la 2a bofetada 
				if ( m_SecondHitReached && !m_SoundPlayed2 )
				{
					m_SoundPlayed2 = true;
					CORE->GetSoundManager()->PlayEvent("Play_EFX_Punch2"); 
				}
				// Sonido de la 2a bofetada fallida
				else if ( !m_SecondHitReached && !m_SoundPlayed2 )
				{
					CORE->GetSoundManager()->PlayEvent("Play_EFX_Slap1"); 
					m_SoundPlayed2 = true;
				}
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Animacion en curso...");
				}
			#endif

			m_pActionStateCallback.Update(_ElapsedTime);
		}
	}
	else
	{
		SetParticlePosition(m_pDeer);
		
		if ( m_pDeer->IsPlayerInsideImpactDistance() ) 
		{
			m_pDeer->GetBehaviors()->SeekOff();
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetStillAttackAnimationState());
			m_pAnimationCallback->StartAnimation();
			m_pActionStateCallback.StartAction();
			
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
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
					LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Nos acercamos primero");
				}
			#endif
		}
	}
}


void CDeerStillAttackState::OnExit( CCharacter* _Character )
{
	// nos volvemos
	/*m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());*/
	
	//LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Salgo!");
	CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerExclaim"); 
}

bool CDeerStillAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
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

void CDeerStillAttackState::SetParticlePosition( CCharacter* _pCharacter )
{
	CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();

	Mat44f l_TransformMatrix		= m44fIDENTITY;
	Mat44f l_RotationMatrix			= m44fIDENTITY;
	Vect4f l_Rotation				= v3fZERO;
	Vect3f l_Translation			= v3fZERO;
	Mat44f l_AnimatedModelTransform = l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("Bip001 R Finger1", l_Translation);
	l_pAnimatedModel->GetBoneRotation("Bip001 R Finger1", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	m_pParticleEmitter1->SetPosition( l_TransformMatrix.GetPos() );
	
	l_TransformMatrix			= m44fIDENTITY;
	l_RotationMatrix			= m44fIDENTITY;
	l_Rotation					= v3fZERO;
	l_Translation				= v3fZERO;
	l_AnimatedModelTransform	= l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("Bip001 R Finger11", l_Translation);
	l_pAnimatedModel->GetBoneRotation("Bip001 R Finger11", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	m_pParticleEmitter2->SetPosition( l_TransformMatrix.GetPos() );
	
	l_TransformMatrix			= m44fIDENTITY;
	l_RotationMatrix			= m44fIDENTITY;
	l_Rotation					= v3fZERO;
	l_Translation				= v3fZERO;
	l_AnimatedModelTransform	= l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("Bip001 L Finger1", l_Translation);
	l_pAnimatedModel->GetBoneRotation("Bip001 L Finger1", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	m_pParticleEmitter3->SetPosition( l_TransformMatrix.GetPos() );
	
	l_TransformMatrix			= m44fIDENTITY;
	l_RotationMatrix			= m44fIDENTITY;
	l_Rotation					= v3fZERO;
	l_Translation				= v3fZERO;
	l_AnimatedModelTransform	= l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("Bip001 L Finger11", l_Translation);
	l_pAnimatedModel->GetBoneRotation("Bip001 L Finger11", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	m_pParticleEmitter4->SetPosition( l_TransformMatrix.GetPos() );
}