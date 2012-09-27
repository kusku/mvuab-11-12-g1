#include "WolfStillAttackState.h"
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
#include "Characters\Enemies\Wolf\AnimationStates\WolfStillAttackAnimationState.h"
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
CWolfStillAttackState::CWolfStillAttackState( CCharacter* _pCharacter )
	: CState					(_pCharacter, "CWolfStillAttackState")
	, m_pWolf					( NULL )
	, m_pAnimationCallback		( NULL )
	, m_pActionStateCallback	( NULL )
	, m_FirstHitDone			( false )
	, m_FirstParticlesHitDone	( false )	
	, m_FirstHitReached			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_STILL_ATTACK_STATE);

	m_pActionStateCallback = new CActionStateCallback(0,1);
}

CWolfStillAttackState::CWolfStillAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState					(_pCharacter, _Name)
	, m_pWolf					( NULL )
	, m_pAnimationCallback		( NULL )
	, m_pActionStateCallback	( NULL )
	, m_FirstHitDone			( false )
	, m_FirstParticlesHitDone	( false )	
	, m_FirstHitReached			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_STILL_ATTACK_STATE);

	m_pActionStateCallback = new CActionStateCallback(0,1);
}


CWolfStillAttackState::~CWolfStillAttackState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
	CHECKED_DELETE(	m_pActionStateCallback );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfStillAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	m_pWolf->SetPlayerHasBeenReached( false );

#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		std::string l_State = WOLF_STILL_ATTACK_STATE;
		CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
	}
#endif

	m_SoundPlayed1			= false;
	m_FirstHitDone			= false;
	m_FirstParticlesHitDone = false;
	m_FirstHitReached		= false;
	m_pAnimationCallback->Init();

	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();

	//CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerExclaim"); 
	m_pActionStateCallback->InitAction(0, m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_STILL_ATTACK_STATE) );
}

void CWolfStillAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Compruebo si la animaci�n a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			//// Aseguro que se toc� o no seg�n la variable que gestiona esto en los golpeos
			m_pWolf->SetPlayerHasBeenReached(m_FirstHitReached);

			if ( m_pWolf->GetPlayerHasBeenReached() )
			{
				// Esto nos permite hacer el par�p� un poco. Situarnos delante la c�mara, una simulaci�n de alejarse por cansancio
				m_pWolf->SetToBeTired(false);

				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackState::Execute->Envio mensaje de tocado");
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CWolfStillAttackState:Execute->El Dispatch es NULL" );
				}
				
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog( ELL_INFORMATION, "CWolfStillAttackState:Execute->Dispatch" );
					}
				#endif
			}
			// Si acaba la animacion pero no estamos en una distancia de poder impactar solo hacemos que se canse
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackState::Execute->Golpeo erratico");
					}
				#endif
			}

			// Incrementamos el n� de ataques hechos --> si llega a un total estar� cansado
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
			// Ahora debemos actualizar las part�culas
			UpdateParticlesPositions(m_pWolf);

			m_pWolf->GetBehaviors()->SeekOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

			//float t = m_pAnimationCallback->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_STILL_ATTACK_STATE);

			// Aqu� comienza el golpeo, la mano est� alzada
			if ( m_pActionStateCallback->IsActionInTime( 0.33f ) && !m_FirstHitDone )
			{
				GetParticleEmitterInstance("WolfBlurHook",m_pWolf->GetName() + "_LeftHand")->EjectParticles();
				m_FirstHitDone = true;		// Ahora ya no entraremos en este condicional
			}

			// Miramos si llegamos a tocar el player en el momento de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.50f ) && !m_SoundPlayed1 )
			{
				// Sonido de la bofetada acertada
				if ( m_pWolf->IsPlayerReached() )
				{
					CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
					m_FirstHitReached = true;
				}
				// Sonido de la bofetada fallida
				else 
				{
					CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Slap1"); 
					m_FirstHitReached = false;
				}
				m_SoundPlayed1 = true;
			}

			//// Miramos si llegamos a tocar el player en el momento de impacto
			//if ( m_pActionStateCallback->IsActionInTime( 0.54f ) && !m_FirstHitReached )
			//{<
			//	// Miramos si alcanzamos al player
			//	if ( m_pWolf->IsPlayerReached() )
			//	{
			//		m_FirstHitReached = true;
			//		UpdateImpact(m_pWolf);
			//	}
			//}
			
			// Trato la animaci�n de impacto
			if ( m_pActionStateCallback->IsActionInTime( 0.54f ) && !m_FirstParticlesHitDone && m_FirstHitReached )
			{
				m_FirstParticlesHitDone = true;
				UpdateImpact(m_pWolf);
				GenerateImpact(m_pWolf );
			}

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackState::Execute->Animacion en curso...");
				}
			#endif

			// hay que cancelar la animaci�n
			/*if ( !m_pWolf->IsPlayerInsideImpactDistance() )
			{
				m_pWolf->GetLogicFSM()->RevertToPreviousState();
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			}*/

			m_pActionStateCallback->Update(_ElapsedTime);
		}
	}
	// Animaci�n no iniciada
	else
	{
		if ( m_pWolf->IsPlayerInsideImpactDistance() ) 
		{
			m_pWolf->GetBehaviors()->SeekOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0) );
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetStillAttackAnimationState());
			m_pAnimationCallback->StartAnimation();
			m_pActionStateCallback->StartAction();

			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackState::Execute->Inicio Animacion");
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
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfStillAttackState::Execute->Nos acercamos primero");
				}
			#endif
		}
	}
}


void CWolfStillAttackState::OnExit( CCharacter* _pCharacter )
{
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();

	//CORE->GetSoundManager()->PlayEvent("Stop_EFX_DeerExclaim"); 
}

bool CWolfStillAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_Character);
		}

		m_pWolf->RestLife(50); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		return true;
	}

	return false;
}

void CWolfStillAttackState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
	SetParticlePosition(_pCharacter, "WolfBlurHook", _pCharacter->GetName() + "_LeftHand" , "Bip01 R Hand"  );
}

void CWolfStillAttackState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleEmitterInstance( "WolfBloodSplash"	, m_pWolf->GetName() + "_BloodSplashLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfExpandWave"	, m_pWolf->GetName() + "_ExpandWaveLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfImpact"		, m_pWolf->GetName() + "_ImpactLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfStreaks"		, m_pWolf->GetName() + "_StreaksLeft")->EjectParticles();
	GetParticleEmitterInstance( "WolfSparks"		, m_pWolf->GetName() + "_SparksLeft")->EjectParticles();
}

void CWolfStillAttackState::UpdateImpact( CCharacter* _pCharacter )
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