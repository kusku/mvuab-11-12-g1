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

#include "Particles\ParticleEmitterInstance.h"

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
	m_FirstHitReached		= false;
	m_SecondHitReached		= false;
	m_FirstParticlesHitDone	= false;
	m_SecondParticlesHitDone= false;
	m_FirstParticlesHitDone	= false;
	m_FirstHitDone			= false;
	m_SecondHitDone			= false;
	m_SoundPlayed1			= false;
	m_SoundPlayed2			= false;
	
	m_pDeer->SetPlayerHasBeenReached( false );
	m_pDeer->SetToBeTired(false);

	//CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerExclaim"); 
	
	m_pAnimationCallback->Init();
	m_pActionStateCallback->InitAction(0, m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE) );
}

void CDeerStillAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
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
				// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio
				m_pDeer->SetToBeTired(true);
				
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

			//float t = m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE);

			// Aquí comienza el golpeo, la mano está alzada
			if ( m_pActionStateCallback->IsActionInTime( 0.2f ) && !m_FirstHitDone )
			{
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );

				GetParticleEmitterInstance( "DeerBlurHook", m_pDeer->GetName() + "_RightHand1")->EjectParticles();
				GetParticleEmitterInstance( "DeerBlurHook", m_pDeer->GetName() + "_RightHand11")->EjectParticles();
								   
				m_FirstHitDone = true;		// Ahora ya no entraremos en este condicional
			}

			if (m_pActionStateCallback->IsActionInTime( 0.30f ) && !m_FirstHitReached)
			{
				//unsigned int n = m_pDeer->GetAnimatedModel()->GetCurrentAnimationTrack(DEER_STILL_ATTACK_STATE);
				if ( m_pDeer->IsPlayerReached() )
				{
					m_FirstHitReached = true;
					if ( DISPATCH != NULL ) 
					{
						DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pDeer->GetID(), m_pDeer->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Envio mensaje de tocado");
					}
					else
					{
						LOGGER->AddNewLog(ELL_ERROR, "CDeerStillAttackState:Execute->El Dispatch es NULL" );
					}
				}

				// Sonido de la 1a bofetada 
				if ( m_FirstHitReached && !m_SoundPlayed1 )
				{
					m_SoundPlayed1 = true;
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
				}
				// Sonido de la 1a bofetada fallida
				else if ( !m_FirstHitReached && !m_SoundPlayed1 )
				{
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
					m_SoundPlayed1 = true;
				}

				// Esto permite correr entre el primer y segundo golpe
				m_pDeer->GetBehaviors()->SeekOn();
				m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_pDeer->GetPlayer()->GetProperties()->GetHitRecoilSpeed());
			}

			// Trato la primera animación de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.36f ) && !m_FirstParticlesHitDone && m_FirstHitReached )
			{
				m_FirstParticlesHitDone = true;
				UpdateImpact(m_pDeer);
				GenerateImpact(m_pDeer, true );
			}

			if ( m_pActionStateCallback->IsActionInTime( 0.7f ) && !m_SecondHitDone )
			{
				// En el segundo golpe paramos
				m_pDeer->GetBehaviors()->SeekOff();
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );

				GetParticleEmitterInstance( "DeerBlurHook", m_pDeer->GetName() + "_LeftHand1")->EjectParticles();
				GetParticleEmitterInstance( "DeerBlurHook", m_pDeer->GetName() + "_LeftHand11")->EjectParticles();

				m_SecondHitDone = true;		// Esto permite ver si ya se hizo el hit y comprobar solo una sola vez y justo en el momento del impacto si se alcanzó el player

				if ( m_pDeer->IsPlayerReached() )
				{
					m_SecondHitReached = true; 
					if ( DISPATCH != NULL ) 
					{
						DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pDeer->GetID(), m_pDeer->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
						LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Envio mensaje de tocado");
					}
					else
					{
						LOGGER->AddNewLog(ELL_ERROR, "CDeerStillAttackState:Execute->El Dispatch es NULL" );
					}
					UpdateImpact(m_pDeer);
				}

				// Sonido de la 2a bofetada 
				if ( m_SecondHitReached && !m_SoundPlayed2 )
				{
					m_SoundPlayed2 = true;
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch2"); 
				}
				// Sonido de la 2a bofetada fallida
				else if ( !m_SecondHitReached && !m_SoundPlayed2 )
				{
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
					m_SoundPlayed2 = true;
				}
			}
			// Trato la segunda animación de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.85f ) && !m_SecondParticlesHitDone && m_SecondHitReached )
			{
				m_SecondParticlesHitDone = true;
				UpdateImpact(m_pDeer);
				GenerateImpact(m_pDeer, false );
			}


			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CDeerStillAttackState::Execute->Animacion en curso...");
				}
			#endif

			m_pActionStateCallback->Update(_ElapsedTime);
			
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
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
	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerExclaim"); 
	if (!m_pDeer)
		return;

	m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_pDeer->GetProperties()->GetMaxSpeed());

	m_pDeer->GetBehaviors()->SeparationOff();
	m_pDeer->GetBehaviors()->CohesionOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();
		
	GetParticleEmitterInstance("DeerBlurHook",_pCharacter->GetName() + "_LeftHand1")->StopEjectParticles();
	GetParticleEmitterInstance("DeerBlurHook",_pCharacter->GetName() + "_LeftHand11")->StopEjectParticles();
	GetParticleEmitterInstance("DeerBlurHook",_pCharacter->GetName() + "_RightHand1")->StopEjectParticles();
	GetParticleEmitterInstance("DeerBlurHook",_pCharacter->GetName() + "_RightHand11")->StopEjectParticles();

	GetParticleEmitterInstance("DeerExpandWave"	,_pCharacter->GetName() + "_ExpandWaveLeft")->StopEjectParticles();
	//GetParticleEmitterInstance("DeerBloodSplash",_pCharacter->GetName() + "_BloodSplashLeft")->StopEjectParticles();
	GetParticleEmitterInstance("DeerImpact"		,_pCharacter->GetName() + "_ImpactLeft")->StopEjectParticles();
	GetParticleEmitterInstance("DeerStreaks"	,_pCharacter->GetName() + "_StreaksLeft")->StopEjectParticles();
	GetParticleEmitterInstance("DeerSparks"		,_pCharacter->GetName() + "_SparksLeft")->StopEjectParticles();

	GetParticleEmitterInstance("DeerExpandWave"	,_pCharacter->GetName()	+ "_ExpandWaveRigth")->StopEjectParticles();
	//GetParticleEmitterInstance("DeerBloodSplash",_pCharacter->GetName()	+ "_BloodSplashRight")->StopEjectParticles();
	GetParticleEmitterInstance("DeerImpact"		,_pCharacter->GetName() + "_ImpactRigth")->StopEjectParticles();
	GetParticleEmitterInstance("DeerStreaks"	,_pCharacter->GetName() + "_StreaksRigth")->StopEjectParticles();
	GetParticleEmitterInstance("DeerSparks"		,_pCharacter->GetName() + "_SparksRigth")->StopEjectParticles();
}					   

bool CDeerStillAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
		}

		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}

void CDeerStillAttackState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, "DeerBlurHook", _pCharacter->GetName() + "_RightHand1" , "Bip001 R Finger1"  );
	SetParticlePosition(_pCharacter, "DeerBlurHook", _pCharacter->GetName() + "_RightHand11", "Bip001 R Finger11" );
	SetParticlePosition(_pCharacter, "DeerBlurHook", _pCharacter->GetName() + "_LeftHand1"  , "Bip001 L Finger1"  );
	SetParticlePosition(_pCharacter, "DeerBlurHook", _pCharacter->GetName() + "_LeftHand11" , "Bip001 L Finger11" );
}



void CDeerStillAttackState::GenerateImpact( CCharacter* _pCharacter, bool _FirstImpact )
{
	if ( _FirstImpact )
	{
		//GetParticleEmitterInstance( "DeerBloodSplash", m_pDeer->GetName() + "_BloodSplashLeft")->EjectParticles();
		GetParticleEmitterInstance( "DeerExpandWave" , m_pDeer->GetName() + "_ExpandWaveLeft")->EjectParticles();
		GetParticleEmitterInstance( "DeerImpact"	 , m_pDeer->GetName() + "_ImpactLeft")->EjectParticles();
		GetParticleEmitterInstance( "DeerStreaks"	 , m_pDeer->GetName() + "_StreaksLeft")->EjectParticles();
		GetParticleEmitterInstance( "DeerSparks"	 , m_pDeer->GetName() + "_SparksLeft")->EjectParticles();
	}
	else
	{
		//GetParticleEmitterInstance( "DeerBloodSplash", m_pDeer->GetName() + "_BloodSplashRight")->EjectParticles();
		GetParticleEmitterInstance( "DeerExpandWave" , m_pDeer->GetName() + "_ExpandWaveRigth")->EjectParticles();
		GetParticleEmitterInstance( "DeerImpact"	 , m_pDeer->GetName() + "_ImpactRigth")->EjectParticles();
		GetParticleEmitterInstance( "DeerStreaks"	 , m_pDeer->GetName() + "_StreaksRigth")->EjectParticles();
		GetParticleEmitterInstance( "DeerSparks"	 , m_pDeer->GetName() + "_SparksRigth")->EjectParticles();
	}
}

void CDeerStillAttackState::UpdateImpact( CCharacter* _pCharacter )
{
	//SetParticlePosition(_pCharacter, "DeerBloodSplash", _pCharacter->GetName() + "_BloodSplashLeft",   "Bip001 R Finger1");
	SetParticlePosition(_pCharacter, "DeerExpandWave",	_pCharacter->GetName() + "_ExpandWaveLeft",	   "Bip001 R Finger1");
	SetParticlePosition(_pCharacter, "DeerImpact",		_pCharacter->GetName() + "_ImpactLeft",		   "Bip001 R Finger1");
	SetParticlePosition(_pCharacter, "DeerStreaks",		_pCharacter->GetName() + "_StreaksLeft",	   "Bip001 R Finger1");
	SetParticlePosition(_pCharacter, "DeerSparks",		_pCharacter->GetName() + "_SparksLeft",		   "Bip001 R Finger1");
																									   
	//SetParticlePosition(_pCharacter, "DeerBloodSplash", _pCharacter->GetName() + "_BloodSplashRight",  "Bip001 L Finger1");
	SetParticlePosition(_pCharacter, "DeerExpandWave",	_pCharacter->GetName() + "_ExpandWaveRigth",   "Bip001 L Finger1");
	SetParticlePosition(_pCharacter, "DeerImpact",		_pCharacter->GetName() + "_ImpactRight",	   "Bip001 L Finger1");
	SetParticlePosition(_pCharacter, "DeerStreaks",		_pCharacter->GetName() + "_StreaksRigth",	   "Bip001 L Finger1");
	SetParticlePosition(_pCharacter, "DeerSparks",		_pCharacter->GetName() + "_SparksRigth",	   "Bip001 L Finger1");
}