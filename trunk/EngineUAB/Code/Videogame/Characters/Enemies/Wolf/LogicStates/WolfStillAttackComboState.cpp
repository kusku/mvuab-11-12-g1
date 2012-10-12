#include "WolfStillAttackComboState.h"
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
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfHitState.h"
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfStillAttackComboAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfStillAttackComboState::CWolfStillAttackComboState( CCharacter* _pCharacter )
	: CState					(_pCharacter, "CWolfStillAttackComboState")
	, m_pWolf					( NULL )
	, m_pAnimationCallback		( NULL )
	, m_pActionStateCallback	( NULL )
	, m_FirstHitDone			( false )
	, m_FirstParticlesHitDone	( false )	
	, m_FirstHitReached			( false )
	, m_SecondHitDone			( false )
	, m_SecondParticlesHitDone	( false )	
	, m_SecondHitReached		( false )
	, m_ThirdHitDone			( false )
	, m_ThirdParticlesHitDone	( false )	
	, m_ThirdHitReached			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_STILL_ATTACK_COMBO_STATE);

	m_pActionStateCallback = new CActionStateCallback(0,1);
}

CWolfStillAttackComboState::CWolfStillAttackComboState( CCharacter* _pCharacter, const std::string &_Name )
	: CState					(_pCharacter, _Name)
	, m_pWolf					( NULL )
	, m_pAnimationCallback		( NULL )
	, m_pActionStateCallback	( NULL )
	, m_FirstHitDone			( false )
	, m_FirstParticlesHitDone	( false )	
	, m_FirstHitReached			( false )
	, m_SecondHitDone			( false )
	, m_SecondParticlesHitDone	( false )	
	, m_SecondHitReached		( false )
	, m_ThirdHitDone			( false )
	, m_ThirdParticlesHitDone	( false )	
	, m_ThirdHitReached			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_STILL_ATTACK_COMBO_STATE);

	m_pActionStateCallback = new CActionStateCallback(0,1);
}


CWolfStillAttackComboState::~CWolfStillAttackComboState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
	CHECKED_DELETE(	m_pActionStateCallback );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfStillAttackComboState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	m_pWolf->SetPlayerHasBeenReached( false );
	m_SoundPlayedScream1 = false;
	m_SoundPlayedScream2 = false;
	m_SoundPlayedScream3 = false;

	/// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio. En este caso no queremos
	// pq hace un desplazamiento que después de este ataque no queremos que haga.
	m_pWolf->SetToBeTired(false);

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = WOLF_STILL_ATTACK_COMBO_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
	}
#endif

	m_SoundPlayed1				= false;
	m_FirstHitDone				= false;
	m_FirstParticlesHitDone		= false;
	m_FirstHitReached			= false;
	m_SoundPlayed2				= false;
	m_SecondHitDone				= false;
	m_SecondParticlesHitDone	= false;
	m_SecondHitReached			= false;
	m_SoundPlayed3				= false;
	m_ThirdHitDone				= false;
	m_ThirdParticlesHitDone		= false;
	m_ThirdHitReached			= false;
	
	m_pAnimationCallback->Init();

	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();

	//CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerExclaim"); 
	m_pActionStateCallback->InitAction(0, m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_STILL_ATTACK_COMBO_STATE) );

	// Metemos más velocidad al ataque 
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetStillAttackSpeed());
	
}

void CWolfStillAttackComboState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			// Aseguro que se tocó o no según las variables que gestionan esto en los golpeos
			m_pWolf->SetPlayerHasBeenReached(m_FirstHitReached || m_SecondHitReached || m_ThirdHitReached);

			if ( m_pWolf->GetPlayerHasBeenReached() )
			{
				// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio
				m_pWolf->SetToBeTired(false);
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Golpeo erratico");
					}
				#endif
			}

			// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
			m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

			// Volvemos a idle
			//m_pWolf->GetLogicFSM()->RevertToPreviousState();
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
				
			m_pWolf->GetBehaviors()->SeekOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			return;
		}
		// NO!! Esto hace que luego corra mientras ataca y queda de pena. Mantengo el comentario para recordarlo.
		// En caso que no estemos en una distancia de ataque nos salimos. Este se comenta pq se prevee que falle y corte la animacion
		/*else if ( !m_pWolf->IsPlayerInsideImpactDistance() )
		{
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());

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
			UpdateParticlesPositions(m_pWolf);

			m_pWolf->GetBehaviors()->SeekOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			// -----------------------------------------------
			// Aquí comienza el golpeo, la 1a mano está alzada
			// -----------------------------------------------
			// Metemos el sonido del primer impacto. Lo he puesto aquí para retrasarlo un poco ya que antes chillaba			un poco temprano y no quedaba tant bien.
			if ( !m_SoundPlayedScream1 && m_pActionStateCallback->IsActionInTime( 0.5f ) )
			{
				CORE->GetSoundManager()->PlayEvent(m_pWolf->GetSpeakerName(), "Play_EFX_Wolf_attack"); 
				m_SoundPlayedScream1 = true;
			}
			
			if ( m_pActionStateCallback->IsActionInTime( 0.33f ) && !m_FirstHitDone )
			{
				GetParticleEmitterInstance("WolfBlurHook",m_pWolf->GetName() + "_LeftHand")->EjectParticles();
				m_FirstHitDone = true;		// Ahora ya no entraremos en este condicional
			}

			// Miramos si llegamos a tocar el player en el momento de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.50f ) && !m_FirstHitReached )
			{
				// Miramos si alcanzamos al player
				if ( m_pWolf->IsPlayerReached() )
				{	
					m_FirstHitReached = true;
					UpdateImpact(m_pWolf);
				
					if ( DISPATCH != NULL ) 
					{
						//DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
						LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Envio mensaje de tocado");
					}
					else
					{
						LOGGER->AddNewLog(ELL_ERROR, "CWolfStillAttackComboState:Execute->El Dispatch es NULL" );
					}
				
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							LOGGER->AddNewLog( ELL_INFORMATION, "CWolfStillAttackComboState:Execute->Dispatch" );
						}
					#endif
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
			if ( m_pActionStateCallback->IsActionInTime( 0.54f ) && !m_FirstParticlesHitDone && m_FirstHitReached )
			{
				m_FirstParticlesHitDone = true;
				UpdateImpact(m_pWolf);
				GenerateImpact(m_pWolf );
			}

			// -----------------------------------------------
			// Aquí comienza el golpeo, la 2a mano está alzada
			// -----------------------------------------------
			//// Metemos el sonido del segundo impacto. Lo he puesto aquí para retrasarlo un poco ya que antes chillaba un poco temprano y no quedaba tant bien.
			//if ( !m_SoundPlayedScream2 && m_pActionStateCallback->IsActionInTime( 0.58f ) )
			//{
			//	CORE->GetSoundManager()->PlayEvent(m_pWolf->GetSpeakerName(), "Play_EFX_Wolf_attack"); 
			//	m_SoundPlayedScream2 = true;
			//}

			//if ( m_pActionStateCallback->IsActionInTime( 0.66f ) && !m_SecondHitDone )
			//{
			//	GetParticleEmitterInstance("WolfBlurHook",m_pWolf->GetName() + "_LeftHand")->EjectParticles();
			//	m_SecondHitDone = true;		// Ahora ya no entraremos en este condicional
			//}

			//// Miramos si llegamos a tocar el player en el momento de impacto
			//if ( m_pActionStateCallback->IsActionInTime( 1.00f ) && !m_SecondHitReached )
			//{
			//	// Miramos si alcanzamos al player
			//	if ( m_pWolf->IsPlayerReached() )
			//	{	
			//		m_SecondHitReached = true;
			//		UpdateImpact(m_pWolf);
			//	
			//		if ( DISPATCH != NULL ) 
			//		{
			//			//DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
			//			LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Envio mensaje de tocado");
			//		}
			//		else
			//		{
			//			LOGGER->AddNewLog(ELL_ERROR, "CWolfStillAttackComboState:Execute->El Dispatch es NULL" );
			//		}
			//	
			//		#if defined _DEBUG
			//			if( CORE->IsDebugMode() )
			//			{
			//				LOGGER->AddNewLog( ELL_INFORMATION, "CWolfStillAttackComboState:Execute->Dispatch" );
			//			}
			//		#endif
			//	}

			//	// Sonido de la bofetada acertada
			//	if ( m_SecondHitReached && !m_SoundPlayed1 )
			//	{
			//		m_SoundPlayed1 = true;
			//		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
			//	}
			//	// Sonido de la bofetada fallida
			//	else if ( !m_SecondHitReached && !m_SoundPlayed1 )
			//	{
			//		CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
			//		m_SoundPlayed1 = true;
			//	}
			//}

			//// Trato la animación de impacto
			//if ( m_pActionStateCallback->IsActionInTime( 0.54f ) && !m_SecondParticlesHitDone && m_SecondHitReached )
			//{
			//	m_SecondParticlesHitDone = true;
			//	UpdateImpact(m_pWolf);
			//	GenerateImpact(m_pWolf );
			//}

			// -----------------------------------------------
			// Aquí comienza el golpeo, la 3a mano está alzada
			// -----------------------------------------------
			//// Metemos el sonido del segundo impacto. Lo he puesto aquí para retrasarlo un poco ya que antes chillaba un poco temprano y no quedaba tant bien.
			//if ( !m_SoundPlayedScream3 && m_pActionStateCallback->IsActionInTime( 0.81f ) )
			//{
			//	CORE->GetSoundManager()->PlayEvent(m_pWolf->GetSpeakerName(), "Play_EFX_Wolf_attack"); 
			//	m_SoundPlayedScream3 = true;
			//}

			//if ( m_pActionStateCallback->IsActionInTime( 0.99f ) && !m_ThirdHitDone )
			//{
			//	GetParticleEmitterInstance("WolfBlurHook",m_pWolf->GetName() + "_LeftHand")->EjectParticles();
			//	m_ThirdHitDone = true;		// Ahora ya no entraremos en este condicional
			//}

			//// Miramos si llegamos a tocar el player en el momento de impacto
			//if ( m_pActionStateCallback->IsActionInTime( 0.50f ) && !m_ThirdHitReached )
			//{
			//	// Miramos si alcanzamos al player
			//	if ( m_pWolf->IsPlayerReached() )
			//	{	
			//		m_ThirdHitReached = true;
			//		UpdateImpact(m_pWolf);
			//	
			//		if ( DISPATCH != NULL ) 
			//		{
			//			//DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
			//			LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Envio mensaje de tocado");
			//		}
			//		else
			//		{
			//			LOGGER->AddNewLog(ELL_ERROR, "CWolfStillAttackComboState:Execute->El Dispatch es NULL" );
			//		}
			//	
			//		#if defined _DEBUG
			//			if( CORE->IsDebugMode() )
			//			{
			//				LOGGER->AddNewLog( ELL_INFORMATION, "CWolfStillAttackComboState:Execute->Dispatch" );
			//			}
			//		#endif
			//	}

			//	// Sonido de la bofetada acertada
			//	if ( m_ThirdHitReached && !m_SoundPlayed1 )
			//	{
			//		m_SoundPlayed1 = true;
			//		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
			//	}
			//	// Sonido de la bofetada fallida
			//	else if ( !m_ThirdHitReached && !m_SoundPlayed1 )
			//	{
			//		CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
			//		m_SoundPlayed1 = true;
			//	}
			//}

			//// Trato la animación de impacto
			//if ( m_pActionStateCallback->IsActionInTime( 0.54f ) && !m_ThirdParticlesHitDone && m_ThirdHitReached )
			//{
			//	m_ThirdParticlesHitDone = true;
			//	UpdateImpact(m_pWolf);
			//	GenerateImpact(m_pWolf );
			//}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Animacion en curso...");
				}
			#endif

			m_pActionStateCallback->Update(_ElapsedTime);
		}
	}
	// Animación no iniciada
	else
	{
		if ( m_pWolf->IsPlayerInsideImpactDistance() ) 
		{
			m_pWolf->GetBehaviors()->SeekOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetStillAttackComboAnimationState());
			m_pAnimationCallback->StartAnimation();
			m_pActionStateCallback->StartAction();

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Inicio Animacion");
				}
			#endif
		}
		else 
		{
			// Nos acercamos 
			// _CCharacter.behaviors:pursuit_on()
			m_pWolf->GetBehaviors()->SeekOn();
			m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
				
			// Rotamos al objetivo y movemos
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackComboState::Execute->Nos acercamos primero");
				}
			#endif
		}
	}
}

void CWolfStillAttackComboState::OnExit( CCharacter* _pCharacter )
{
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();

	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetMaxSpeed());

	CORE->GetSoundManager()->PlayEvent("Stop_EFX_Wolf_attack"); 
}

bool CWolfStillAttackComboState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	return m_pWolf->CallHitState(_pCharacter, _Telegram);
}

void CWolfStillAttackComboState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, "WolfBlurHook", _pCharacter->GetName() + "_LeftHand" , "Bip01 R Hand"  );
}

void CWolfStillAttackComboState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleEmitterInstance( "WolfBloodSplash"	, m_pWolf->GetName() + "_BloodSplashLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfExpandWave"	, m_pWolf->GetName() + "_ExpandWaveLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfImpact"		, m_pWolf->GetName() + "_ImpactLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfStreaks"		, m_pWolf->GetName() + "_StreaksLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfSparks"		, m_pWolf->GetName() + "_SparksLeft")->EjectParticles();
}

void CWolfStillAttackComboState::UpdateImpact( CCharacter* _pCharacter )
{
	//SetParticlePosition(_pCharacter, "WolfBloodSplash",	_pCharacter->GetName() + "_BloodSplashLeft", "Bip001 R Hand");
	/*SetParticlePosition(_pCharacter, "WolfExpandWave",	_pCharacter->GetName() + "_ExpandWaveLeft",  "Bip01 R Hand");
	SetParticlePosition(_pCharacter, "WolfImpact",		_pCharacter->GetName() + "_ImpactLeft",		 "Bip01 R Hand");
	SetParticlePosition(_pCharacter, "WolfStreaks",		_pCharacter->GetName() + "_StreaksLeft",	 "Bip01 R Hand");
	SetParticlePosition(_pCharacter, "WolfSparks",		_pCharacter->GetName() + "_SparksLeft",		 "Bip01 R Hand");*/

	Vect3f l_Pos = _pCharacter->GetPlayer()->GetPosition();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "WolfExpandWave",	_pCharacter->GetName() + "_ExpandWaveLeft",  "", l_Pos);
	SetParticlePosition(_pCharacter, "WolfImpact",		_pCharacter->GetName() + "_ImpactLeft",		 "", l_Pos);
	SetParticlePosition(_pCharacter, "WolfStreaks",		_pCharacter->GetName() + "_StreaksLeft",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "WolfSparks",		_pCharacter->GetName() + "_SparksLeft",		 "", l_Pos);

}