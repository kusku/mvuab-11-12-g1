#include "RabbitStillAttackState.h"
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
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitHitState.h"
#include "RabbitIdleState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitStillAttackAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitRunAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Particles\ParticleEmitterInstance.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitStillAttackState::CRabbitStillAttackState( CCharacter* _pCharacter )
	: CState					(_pCharacter, "CRabbitStillAttackState")
	, m_pRabbit					( NULL )
	, m_pAnimationCallback		( NULL )
	, m_pActionStateCallback	( NULL )
	, m_FirstHitDone			( false )
	, m_FirstParticlesHitDone	( false )	
	, m_FirstHitReached			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_STILL_ATTACK_STATE);
	m_pActionStateCallback = new CActionStateCallback(0,1);
}

CRabbitStillAttackState::CRabbitStillAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState					(_pCharacter, _Name)
	, m_pRabbit					( NULL )
	, m_pAnimationCallback		( NULL )
	, m_pActionStateCallback	( NULL )
	, m_FirstHitDone			( false )
	, m_FirstParticlesHitDone	( false )	
	, m_FirstHitReached			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_STILL_ATTACK_STATE);
	m_pActionStateCallback = new CActionStateCallback(0,1);
}


CRabbitStillAttackState::~CRabbitStillAttackState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
	CHECKED_DELETE(	m_pActionStateCallback );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitStillAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_pRabbit->SetPlayerHasBeenReached( false );
	m_SoundPlayedScream = false;

	/// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio. En este caso no queremos
	// pq hace un desplazamiento que después de este ataque no queremos que haga.
	m_pRabbit->SetToBeTired(false);

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = RABBIT_STILL_ATTACK_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
	}
#endif

	m_SoundPlayed1			= false;
	m_FirstHitDone			= false;
	m_FirstParticlesHitDone = false;
	m_FirstHitReached		= false;
	m_pAnimationCallback->Init();

	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();

	//CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerExclaim"); 
	m_pActionStateCallback->InitAction(0, m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_STILL_ATTACK_STATE) );
}

void CRabbitStillAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			//// Aseguro que se tocó o no según la variable que gestiona esto en los golpeos
			m_pRabbit->SetPlayerHasBeenReached(m_FirstHitReached);

			if ( m_pRabbit->GetPlayerHasBeenReached() )
			{
				// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio
				m_pRabbit->SetToBeTired(true);

				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pRabbit->GetID(), m_pRabbit->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitStillAttackState::Execute->Envio mensaje de tocado");
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CRabbitStillAttackState:Execute->El Dispatch es NULL" );
				}
				
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog( ELL_INFORMATION, "CRabbitStillAttackState:Execute->Dispatch" );
					}
				#endif
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitStillAttackState::Execute->Golpeo erratico");
					}
				#endif
			}

			// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
			m_pRabbit->SetHitsDone(m_pRabbit->GetHitsDone() + 1);

			// Volvemos a idle
			//m_pRabbit->GetLogicFSM()->RevertToPreviousState();
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
				
			m_pRabbit->GetBehaviors()->SeekOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

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
					CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), "Allunyat!");
				}
			#endif

			return;
		}*/
		// En otro caso actualizamos el tiempo de animacion
		else
		{
			// Ahora debemos actualizar las partículas
			UpdateParticlesPositions(m_pRabbit);

			m_pRabbit->GetBehaviors()->SeekOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			// Metemos el sonido. Lo he puesto aquí para retrasarlo un poco ya que antes chillaba un poco temprano y no quedaba tant bien.
			if ( !m_SoundPlayedScream && m_pActionStateCallback->IsActionInTime( 0.30f ) )
			{
				CORE->GetSoundManager()->PlayEvent(m_pRabbit->GetSpeakerName(), "Play_EFX_Rabbit_still_attack"); 
				m_SoundPlayedScream = true;
			}

			//float t = m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE);

			// Aquí comienza el golpeo, la mano está alzada
			if ( m_pActionStateCallback->IsActionInTime( 0.33f ) && !m_FirstHitDone )
			{
				GetParticleEmitterInstance("RabbitBlurHook",m_pRabbit->GetName() + "_LeftHand")->EjectParticles();
								   
				m_FirstHitDone = true;		// Ahora ya no entraremos en este condicional
			}

			// Miramos si llegamos a tocar el player en el momento de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.5f ) && !m_FirstHitReached )
			{
				// Miramos si alcanzamos al player
				if ( m_pRabbit->IsPlayerReached() )
				{
					m_FirstHitReached = true;
					UpdateImpact(m_pRabbit);
				}

				// Sonido de la bofetada acertada
				if ( m_FirstHitReached && !m_SoundPlayed1 )
				{
					m_SoundPlayed1 = true;
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
				}
				// Sonido de la bofetada fallida
				else if ( !m_FirstHitReached && !m_SoundPlayed1 )
				{
					CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
					m_SoundPlayed1 = true;
				}
			}
			
			// Trato la animación de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.5f ) && !m_FirstParticlesHitDone && m_FirstHitReached )
			{
				m_FirstParticlesHitDone = true;
				UpdateImpact(m_pRabbit);
				GenerateImpact(m_pRabbit );
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitStillAttackState::Execute->Animacion en curso...");
				}
			#endif

			// hay que cancelar la animación
			/*if ( !m_pRabbit->IsPlayerInsideImpactDistance() )
			{
				m_pRabbit->GetLogicFSM()->RevertToPreviousState();
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			}*/

			m_pActionStateCallback->Update(_ElapsedTime);
		}
	}
	else
	{
		if ( m_pRabbit->IsPlayerInsideImpactDistance() ) 
		{
			m_pRabbit->GetBehaviors()->SeekOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetStillAttackAnimationState());
			m_pAnimationCallback->StartAnimation();
			m_pActionStateCallback->StartAction();

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitStillAttackState::Execute->Inicio Animacion");
				}
			#endif
		}
		else 
		{
			// Nos acercamos 
			// _CCharacter.behaviors:pursuit_on()
			m_pRabbit->GetBehaviors()->SeekOn();
			m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
				
			// Rotamos al objetivo y movemos
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitStillAttackState::Execute->Nos acercamos primero");
				}
			#endif
		}
	}
}

void CRabbitStillAttackState::OnExit( CCharacter* _pCharacter )
{
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();

	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerExclaim"); 
}

bool CRabbitStillAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
		}

		m_pRabbit->RestLife(50); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		return true;
	}

	return false;
}

void CRabbitStillAttackState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, "RabbitBlurHook", _pCharacter->GetName() + "_LeftHand" , "Bip001 R Finger1"  );
	//SetParticlePosition(_pCharacter, "RabbitBlurHook", _pCharacter->GetName() + "_LeftHand1", "Bip001 R Finger11" );
}

void CRabbitStillAttackState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleEmitterInstance( "RabbitBloodSplash"	, m_pRabbit->GetName() + "_BloodSplashLeft")->EjectParticles();
	GetParticleEmitterInstance( "RabbitExpandWave"	, m_pRabbit->GetName() + "_ExpandWaveLeft")->EjectParticles();
	GetParticleEmitterInstance( "RabbitImpact"		, m_pRabbit->GetName() + "_ImpactLeft")->EjectParticles();
	GetParticleEmitterInstance( "RabbitStreaks"		, m_pRabbit->GetName() + "_StreaksLeft")->EjectParticles();
	GetParticleEmitterInstance( "RabbitSparks"		, m_pRabbit->GetName() + "_SparksLeft")->EjectParticles();
}

void CRabbitStillAttackState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPlayer()->GetPosition();
	l_Pos.y += _pCharacter->GetPlayer()->GetProperties()->GetHeightController();

	SetParticlePosition(_pCharacter, "RabbitExpandWave",	_pCharacter->GetName() + "_ExpandWaveLeft",  "", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitImpact",		_pCharacter->GetName() + "_ImpactLeft",		 "", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitStreaks",		_pCharacter->GetName() + "_StreaksLeft",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitSparks",		_pCharacter->GetName() + "_SparksLeft",		 "", l_Pos);

	////SetParticlePosition(_pCharacter, "RabbitBloodSplash",	_pCharacter->GetName() + "_BloodSplashLeft", "Bip001 R Finger1");
	//SetParticlePosition(_pCharacter, "RabbitExpandWave",	_pCharacter->GetName() + "_ExpandWaveLeft",  "Bip001 R Finger1");
	//SetParticlePosition(_pCharacter, "RabbitImpact",		_pCharacter->GetName() + "_ImpactLeft",		 "Bip001 R Finger1");
	//SetParticlePosition(_pCharacter, "RabbitStreaks",		_pCharacter->GetName() + "_StreaksLeft",	 "Bip001 R Finger1");
	//SetParticlePosition(_pCharacter, "RabbitSparks",		_pCharacter->GetName() + "_SparksLeft",		 "Bip001 R Finger1");
}