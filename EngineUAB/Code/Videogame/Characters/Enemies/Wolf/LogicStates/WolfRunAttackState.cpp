#include "WolfRunAttackState.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Math\Vector3.h"
#include "SoundManager.h"
#include "Utils\Timer.h"
#include <boost\math\special_functions\fpclassify.hpp>

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
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunRunAttackAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfPreparedRunAttackAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfImpactRunAttackAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\ParticleEmitterManager.h"
#include "Particles\ParticleEmitterSystemInstance.h"

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
	, m_pActionStateCallback( 0, 1 )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_ONLY_IMPACT_RUN_ATTACK_STATE);
}

CWolfRunAttackState::CWolfRunAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_pActionStateCallback( 0, 1 )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_ONLY_IMPACT_RUN_ATTACK_STATE);
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

	CORE->GetSoundManager()->PlayEvent("Play_EFX_Wolf_Run_Attack"); 

	m_pWolf->SetPlayerHasBeenReached( false );
	m_playerPushed = false;

	/// Esto nos permite hacer el par�p� un poco. Situarnos delante la c�mara, una simulaci�n de alejarse por cansancio. En este caso no queremos
	// pq hace un desplazamiento que despu�s de este ataque no queremos que haga.
	m_pWolf->SetToBeTired(false);

	// Metemos m�s velocidad al ataque i menos massa para acelerar m�s 
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetRunAttackSpeed());
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));

	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	/*m_PlayerInitialPosition		= m_pWolf->GetPlayer()->GetSteeringEntity()->GetPosition();
	Vect3f l_Position			= m_pWolf->GetPosition();
	Vect3f l_RelativePosition	= m_PlayerInitialPosition - m_pWolf->GetPosition();
	Vect3f l_RelativePositionN	= l_RelativePosition.GetNormalized();
	m_InitialDistance			= m_pWolf->GetDistanceToPlayer();
	if ( m_InitialDistance <= 9 )
	{
		m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( 9.f) );
	}
	else
	{
		m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 0.f) );
	}*/

	// Activo el seek a saco a una posici� en el momento de inicio de ataque
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_FinalAttackPosition);
	m_pWolf->GetBehaviors()->SeparationOn();
	m_pWolf->GetBehaviors()->CohesionOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();

	m_AnimationDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_RUN_ATTACK_STATE);
	m_pActionStateCallback.InitAction(0, m_AnimationDuration);
	
	m_pAnimationCallback->Init();
	
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = WOLF_RUN_ATTACK_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfRunAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		if (!m_pWolf)
			return;
	}
	
	UpdateImpact(m_pWolf);
	GetParticleEmitterInstance("WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud")->EjectParticles();
	
	//if ( m_pActionStateCallback.IsActionStarted())
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Si el player no estaba ya alcanzado pero ahora se alcanza
		if ( !m_pWolf->GetPlayerHasBeenReached() && m_pWolf->IsPlayerReached() )
		{
			m_pWolf->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Wolf_steps"); 

			UpdateImpact(m_pWolf);
			GenerateImpact(m_pWolf);

			if ( DISPATCH != NULL ) 
			{
				DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Push, NO_ADDITIONAL_INFO );
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION, "CWolfRunAttackState:Execute->Dispatch" );
					}
				#endif
			}
			else
			{
				LOGGER->AddNewLog(ELL_ERROR, "CWolfRunAttackState:Execute->El Dispatch es NULL" );
			}
		}

		if ( m_pAnimationCallback->IsAnimationFinished() )
		//if ( m_pActionStateCallback.IsActionFinished())
		{
			// Si encontr� el player por delante finalizo golpeando
			if ( m_pWolf->GetPlayerHasBeenReached() )
			{
				// Esto nos permite hacer el par�p� un poco. Situarnos delante la c�mara, una simulaci�n de alejarse por cansancio. En este caso no queremos
				// pq hace un desplazamiento que despu�s de este ataque no queremos que haga.
				m_pWolf->SetToBeTired(true);
			}
			// Sin� impact� no hago nada
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION,"CWolfRunAttackState::Execute->Golpeo erratico");
					}
				#endif
			}
				
			// Incrementamos el n� de ataques hechos --> si llega a un total estar� cansado
			m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

			// Volvemos al estado anterior
			m_pWolf->GetBehaviors()->SeekOff();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
				
			return;
		}
		// Si no ha finalizado la animaci�n. Aqu� entra cuando ya pegamos...
		else
		{
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION, "CWolfRunAttackState:Execute->Animation Not finished yet" );
				}
			#endif
			
			// Comprobamos que le pegamos
			if ( m_pWolf->GetPlayerHasBeenReached() )
			{
				m_pWolf->GetBehaviors()->SeekOff();
				m_PlayerInitialPosition = Vect3f(0,0,0);
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			}	
			else
			{
				m_pWolf->GetBehaviors()->SeekOn();
				m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
			}
		
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		}
	}
	// Si l'animaci�n no se ha iniciado
	else
	{
		// Analizo la distancia. Depende de ella que metamos una animaci�n u otra
		float l_Distance = m_pWolf->GetDistanceToPlayer();

		// En el OnEnter hemos puesto la velocidad m�xima
		m_pWolf->GetBehaviors()->SeekOn();
		m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());

		// Estamos lejos y corremos
		if ( l_Distance > m_pWolf->GetProperties()->GetAproximationDistance() ) 
		{
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunRunAttackAnimationState());
			//CORE->GetSoundManager()->PlayEvent("Play_EFX_Wolf_Run_Attack"); 
		}
		//else if ( ( l_Distance <= m_pWolf->GetProperties()->GetAproximationDistance() ) && ( l_Distance >  m_pWolf->GetProperties()->GetImpactDistance() ) )
		//{
		//	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetPreparedRunAttackAnimationState());
		//	//CORE->GetSoundManager()->PlayEvent("Play_EFX_Wolf_Run_Attack"); 
		//}
		else if ( l_Distance <= ( m_pWolf->GetProperties()->GetAproximationDistance() ) )
		{
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetImpactRunAttackAnimationState());
			m_pAnimationCallback->StartAnimation();
			//m_pActionStateCallback.StartAction();
			if ( m_pWolf->GetPlayerHasBeenReached() == false && m_pWolf->IsPlayerReached() )
			{
				m_pWolf->SetPlayerHasBeenReached(true);
				CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Wolf_steps"); 

				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Push, NO_ADDITIONAL_INFO );
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							LOGGER->AddNewLog(ELL_INFORMATION, "CWolfRunAttackState:Execute->Dispatch" );
						}
					#endif
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CWolfRunAttackState:Execute->El Dispatch es NULL" );
				}
			}
		}	

		m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
		m_pWolf->MoveTo2(  m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "CWolfRunAttackState:Execute->Correm al objectiu i embestim" );
			}
		#endif
	}
}


void CWolfRunAttackState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		if (!m_pWolf)
			return;
	}

	// Quitamos el behaviur
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->SeparationOff();
	m_pWolf->GetBehaviors()->CohesionOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();

	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetMaxSpeed());
	CORE->GetSoundManager()->PlayEvent("Stop_EFX_Wolf_run_attack"); 

	// Finalizamos las part�culas
	StopImpact(m_pWolf);
}

bool CWolfRunAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	return m_pWolf->CallHitState(_pCharacter, _Telegram);
}

void CWolfRunAttackState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleGroupInstance("WolfRunAttack","WolfRunAttack")->GetEmitterInstance("...");
	GetParticleEmitterInstance("WolfImpact",		 _pCharacter->GetName() + "_WolfRunImpact")->EjectParticles();
	GetParticleEmitterInstance("WolfExpandWave",  _pCharacter->GetName() + "_WolfRunExpandWave")->EjectParticles();
	/*GetParticleEmitterInstance("WolfRunAttackRay",   _pCharacter->GetName() + "_WolfRunAttackRay")->EjectParticles();*/
	GetParticleEmitterInstance("WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud")->EjectParticles();
}

void CWolfRunAttackState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "WolfImpact",	   _pCharacter->GetName() + "_WolfRunImpact",		"", l_Pos);
	SetParticlePosition(_pCharacter, "WolfExpandWave",  _pCharacter->GetName() + "_WolfRunExpandWave",	"", l_Pos);
	/*SetParticlePosition(_pCharacter, "WolfRunAttackRay",   _pCharacter->GetName() + "_WolfRunAttackRay",    "", l_Pos);*/
	SetParticlePosition(_pCharacter, "WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud",	"", _pCharacter->GetPosition());
}

void CWolfRunAttackState::StopImpact( CCharacter* _pCharacter )
{
	//GetParticleGroupInstance("WolfRunAttack","WolfRunAttack")->GetEmitterInstance("...")->StopEjectParticles
	GetParticleEmitterInstance("WolfImpact",		 _pCharacter->GetName() + "_WolfRunImpact")->StopEjectParticles();
	GetParticleEmitterInstance("WolfExpandWave",  _pCharacter->GetName() + "_WolfRunExpandWave")->StopEjectParticles();
	/*GetParticleEmitterInstance("WolfRunAttackRay",   _pCharacter->GetName() + "_WolfRunAttackRay")->StopEjectParticles();*/
	GetParticleEmitterInstance("WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud")->StopEjectParticles();
}