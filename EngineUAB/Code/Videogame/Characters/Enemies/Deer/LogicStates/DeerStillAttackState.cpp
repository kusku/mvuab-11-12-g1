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
#include "Callbacks\State\ActionStateCallback.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Particles\ParticleEmitter.h"
#include "Particles\ParticleEmitterManager.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerStillAttackState::CDeerStillAttackState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CDeerStillAttackState")
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_pActionStateCallback( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), DEER_STILL_ATTACK_STATE);
	m_pActionStateCallback = new CActionStateCallback(0,1);
}

CDeerStillAttackState::CDeerStillAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pDeer				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_pActionStateCallback( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), DEER_STILL_ATTACK_STATE);
	m_pActionStateCallback = new CActionStateCallback(0,1);
}


CDeerStillAttackState::~CDeerStillAttackState(void)
{
	m_pDeer					= NULL;
	m_pAnimationCallback	= NULL;
	CHECKED_DELETE(	m_pActionStateCallback );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerStillAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_pDeer->GetBehaviors()->SeparationOn();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOn();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOn();

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = DEER_STILL_ATTACK_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
	}
#endif

	//Lanza el sistema de partículas
	//UpdateParticlesPositions(m_pDeer);
	
	// Gestión de sonidos e impacto
	m_FirstHitReached	= false;
	m_SecondHitReached	= false;
	m_FirstHitDone		= false;
	m_SecondHitDone		= false;
	m_SoundPlayed1		= false;
	m_SoundPlayed2		= false;
	
	m_pDeer->SetPlayerHasBeenReached( false );
	
	//CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerExclaim"); 
	
	if ( m_pDeer->GetName() == "enemy29") 
	{
		bool l = m_pAnimationCallback->IsAnimationStarted();
	}


	m_pAnimationCallback->Init();
	m_pActionStateCallback->InitAction(0, m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE) );
}

void CDeerStillAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	
	if ( m_pDeer->GetName() == "enemy29") 
	{
		bool l = m_pAnimationCallback->IsAnimationStarted();
	}

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
			// Ahora debemos actualizar las partículas
			UpdateParticlesPositions(m_pDeer);

			LOGGER->AddNewLog(ELL_WARNING, "CDeerStillAttackState::Execute->Aquí actualizo efectos de %s ", m_pDeer->GetName().c_str() );

			m_pDeer->GetBehaviors()->SeekOff();
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			//float t = m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE);

			if ( m_pActionStateCallback->IsActionInTime( 0.2f ) && !m_FirstHitDone )
			{
				GetParticleEmitter(m_pDeer->GetName() + "_RightHand1")->EjectParticles();
				GetParticleEmitter(m_pDeer->GetName() + "_RightHand11")->EjectParticles();
				GetParticleEmitter(m_pDeer->GetName() + "_Impact1")->EjectParticles();
								   
				m_FirstHitDone = true;		// Ahora ya no entraremos en este condicional

				if ( m_pDeer->IsPlayerReached() )
				{
					m_FirstHitReached = true;
				}

				// Sonido de la 1a bofetada 
				if ( m_FirstHitReached && !m_SoundPlayed1 )
				{
					m_SoundPlayed1 = true;
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
					//GetParticleEmitter("Impact1")->EjectParticles();
				}
				// Sonido de la 1a bofetada fallida
				else if ( !m_FirstHitReached && !m_SoundPlayed1 )
				{
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
					m_SoundPlayed1 = true;
				}
			}

			if ( m_pActionStateCallback->IsActionInTime( 0.7f ) && !m_SecondHitDone )
			{
				GetParticleEmitter(m_pDeer->GetName() + "_LeftHand1")->EjectParticles();
				GetParticleEmitter(m_pDeer->GetName() + "_LeftHand11")->EjectParticles();
				
				m_SecondHitDone = true;		// Esto permite ver si ya se hizo el hit y comprobar solo una sola vez y justo en el momento del impacto si se alcanzó el player

				if ( m_pDeer->IsPlayerReached() )
				{
					m_SecondHitReached = true; 
				}

				// Sonido de la 2a bofetada 
				if ( m_SecondHitReached && !m_SoundPlayed2 )
				{
					m_SoundPlayed2 = true;
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch2"); 
					//GetParticleEmitter("Impact1")->EjectParticles();
				}
				// Sonido de la 2a bofetada fallida
				else if ( !m_SecondHitReached && !m_SoundPlayed2 )
				{
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
					m_SoundPlayed2 = true;
				}
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Animacion en curso...");
				}
			#endif

			m_pActionStateCallback->Update(_ElapsedTime);
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

			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetStillAttackAnimationState());
			m_pAnimationCallback->StartAnimation();
			m_pActionStateCallback->InitAction();
			m_pActionStateCallback->StartAction();
			
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_WARNING,"CDeerStillAttackState::Execute->Inicio Animacion %s ", m_pDeer->GetName().c_str() );
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

			m_pAnimationCallback->Init();
			m_pActionStateCallback->InitAction();

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_WARNING,"CDeerStillAttackState::Execute->Nos acercamos primero %s ", m_pDeer->GetName().c_str() );
				}
			#endif
		}
	}
}


void CDeerStillAttackState::OnExit( CCharacter* _pCharacter )
{
	// nos volvemos
	/*m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());*/
	
	//LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Salgo!");
	CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerExclaim"); 

	m_pDeer->GetBehaviors()->SeparationOff();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();

	GetParticleEmitter(_pCharacter->GetName() + "_LeftHand1")->StopEjectParticles();
	GetParticleEmitter(_pCharacter->GetName() + "_LeftHand11")->StopEjectParticles();
	GetParticleEmitter(_pCharacter->GetName() + "_RightHand1")->StopEjectParticles();
	GetParticleEmitter(_pCharacter->GetName() + "_RightHand11")->StopEjectParticles();
	GetParticleEmitter(_pCharacter->GetName() + "_Impact1")->StopEjectParticles();
}					   

bool CDeerStillAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
		}

		m_pDeer->RestLife(50); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}

void CDeerStillAttackState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, _pCharacter->GetName() + "_RightHand1" , "Bip001 R Finger1"  );
	SetParticlePosition(_pCharacter, _pCharacter->GetName() + "_RightHand11", "Bip001 R Finger11" );
	SetParticlePosition(_pCharacter, _pCharacter->GetName() + "_LeftHand1"  , "Bip001 L Finger1"  );
	SetParticlePosition(_pCharacter, _pCharacter->GetName() + "_LeftHand11" , "Bip001 L Finger11" );
	SetParticlePosition(_pCharacter, _pCharacter->GetName() + "_Impact1", "", _pCharacter->GetPlayer()->GetPosition() - _pCharacter->GetPlayer()->GetFront() );
}

void CDeerStillAttackState::SetParticlePosition( CCharacter* _pCharacter, const std::string &_ParticlesName, const std::string &_Bone, const Vect3f &_Position )
{
	if ( _Bone.compare( "" ) != 0 )
	{
		CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();

		Mat44f l_TransformMatrix		= m44fIDENTITY;
		Mat44f l_RotationMatrix			= m44fIDENTITY;
		Vect4f l_Rotation				= v3fZERO;
		Vect3f l_Translation			= v3fZERO;
		Mat44f l_AnimatedModelTransform = l_pAnimatedModel->GetTransform();

		l_pAnimatedModel->GetBonePosition(_Bone, l_Translation);
		l_pAnimatedModel->GetBoneRotation(_Bone, l_Rotation);

		l_TransformMatrix.Translate(l_Translation);
		l_RotationMatrix.SetFromQuaternion(l_Rotation);

		l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

		GetParticleEmitter(_ParticlesName)->SetPosition( l_TransformMatrix.GetPos() );
	}
	else 
	{
		GetParticleEmitter(_ParticlesName)->SetPosition( _Position );
	}
}