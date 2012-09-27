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
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAttackAnimationState.h"
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

	m_pWolf->SetPlayerHasBeenReached( false );
	m_playerPushed = false;

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetRunAttackSpeed());
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));

	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_PlayerInitialPosition		= m_pWolf->GetPlayer()->GetSteeringEntity()->GetPosition();
	Vect3f l_Position			= m_pWolf->GetPosition();
	Vect3f l_RelativePosition	= m_PlayerInitialPosition - m_pWolf->GetPosition();
	Vect3f l_RelativePositionN	= l_RelativePosition.GetNormalized();
	m_InitialDistance			= m_pWolf->GetDistanceToPlayer();
	if ( m_InitialDistance <= 7 )
	{
		m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 7.f) );
	}
	else
	{
		m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 0.f) );
	}

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_FinalAttackPosition);
	m_pWolf->GetBehaviors()->SeparationOn();
	m_pWolf->GetBehaviors()->CohesionOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();

	m_AnimationDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_RUN_ATTACK_STATE);

	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent("Play_EFX_Wolf_Run_Attack"); 

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
	UpdateImpact(m_pWolf);
	GetParticleEmitterInstance("WolfRunAttackCloud", _pCharacter->GetName() + "_RunAttackCloud")->EjectParticles();
	
	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Si el player no estaba ya alcanzado pero ahora se alcanza
		if ( !m_pWolf->GetPlayerHasBeenReached() && m_pWolf->IsPlayerReached() )
		{
			m_pWolf->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Run_Attack_Charged"); 

			UpdateImpact(m_pWolf);
			GenerateImpact(m_pWolf);
		}
			
		// Compruebo si la animación ha finalizado. Trato también el caso que me pase de la posición. Podria ser que la animación tardara demasiado y esto enviaria el Wolf demasiado lejos
		//float l_CurrentDistance = m_pWolf->GetSteeringEntity()->GetPosition().Distance(m_FinalAttackPosition);
		//float l_PreviousDistance = m_pWolf->GetSteeringEntity()->GetPreviousPosition().Distance(m_FinalAttackPosition);
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
			// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio. En este caso no queremos
			// pq hace un desplazamiento que después de este ataque no queremos que haga.
			m_pWolf->SetToBeTired(false);

			// Si encontré el player por delante finalizo golpeando
			if ( m_pWolf->GetPlayerHasBeenReached() )
			{
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					//m_pAnimationCallback->Init();
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
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION,"CWolfRunAttackState::Execute->Golpeo erratico");
					}
				#endif
			}
				
			// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
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
		// Si no ha finalizado la animación...
		else
		{
			// Si encuentro el player por delante me lo llevo
			if ( m_pWolf->GetPlayerHasBeenReached() )
			{
				if ( !m_playerPushed )
				{
					Vect3f l_Vel = m_pWolf->GetSteeringEntity()->GetVelocity();
					l_Vel.Normalize();
					bool l_isNan  = boost::math::isnan( l_Vel.x );
					if ( l_isNan )
					{
						l_Vel = m_pWolf->GetSteeringEntity()->GetHeading();
						l_isNan  = boost::math::isnan( l_Vel.x );
						if ( l_isNan )
						{
							l_Vel = Vect3f(0,0,0);
							m_AditionalInfo.Speed = m_pWolf->GetProperties()->GetRunAttackSpeed();
						}
						else
						{	
							l_Vel *= m_pWolf->GetProperties()->GetMaxSpeed();
						}
					}
					else
					{
						l_Vel *= m_pWolf->GetProperties()->GetMaxSpeed();
					}
					m_AditionalInfo.Direccion	= l_Vel;
					m_AditionalInfo.ElapsedTime = _ElapsedTime;
					GenerateImpact(m_pWolf);
				}
					
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Push, &m_AditionalInfo );
					m_playerPushed = true;
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CWolfRunAttackState:Execute->El Dispatch es NULL" );
				}
			}
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION, "CWolfRunAttackState:Execute->Animation Not finished yet" );
					}
				#endif
			}
		
			// Comprobamos que no nos hemos pasado de la posición final
			Vect2f l_EnemyPosicion  = Vect2f ( m_pWolf->GetPosition().x, m_pWolf->GetPosition().z);
			Vect2f l_FinalPosicion	= Vect2f ( m_FinalAttackPosition.x, m_FinalAttackPosition.z);
			if ( l_EnemyPosicion. Distance( l_FinalPosicion ) <= 0.5f )
			{
				m_pWolf->GetBehaviors()->SeekOff();
				m_PlayerInitialPosition = Vect3f(0,0,0);
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			}	
			else
			{
				m_pWolf->GetBehaviors()->SeekOn();
				m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_FinalAttackPosition);
			}
		
			// No Rotamos al objetivo y pero si movemos. Esto dará sensación de golpear allí donde estava el target cuando inicie el ataque
			//_CCharacter:face_to( self.target_position, _elapsed_time )
			m_pWolf->FaceTo( m_FinalAttackPosition, _ElapsedTime );
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		}
	}
	// Si l'animación no se ha iniciado
	else
	{
		m_pWolf->GetBehaviors()->SeekOn	();
		m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_PlayerInitialPosition);
		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "CWolfRunAttackState:Execute->Correm al objectiu" );
			}
		#endif
			
		// Ahora azitamos y empezamos la animación de ataque
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAttackAnimationState());
		m_pAnimationCallback->StartAnimation();

		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->FaceTo( m_FinalAttackPosition, _ElapsedTime );
		m_pWolf->MoveTo2(  m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

		if ( m_pWolf->GetPlayerHasBeenReached() == false && m_pWolf->IsPlayerReached() )
		{
			m_pWolf->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Run_Attack_Charged"); 
		}
	}
}


void CWolfRunAttackState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	if (!m_pWolf)
		return;

	// Quitamos el behaviur
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->SeparationOff();
	m_pWolf->GetBehaviors()->CohesionOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();

	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetMaxSpeed());
	CORE->GetSoundManager()->PlayEvent("Stop_EFX_Wolf_run_attack"); 

	// Finalizamos las partículas
	StopImpact(m_pWolf);
}

bool CWolfRunAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(50); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}

	return false;
}

void CWolfRunAttackState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleGroupInstance("WolfRunAttack","WolfRunAttack")->GetEmitterInstance("...");
	/*GetParticleEmitterInstance("WolfRunImpact",		 _pCharacter->GetName() + "_WolfRunImpact")->EjectParticles();
	GetParticleEmitterInstance("WolfRunExpandWave",  _pCharacter->GetName() + "_WolfRunExpandWave")->EjectParticles();
	GetParticleEmitterInstance("WolfRunAttackRay",   _pCharacter->GetName() + "_WolfRunAttackRay")->EjectParticles();
	GetParticleEmitterInstance("WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud")->EjectParticles();*/
}

void CWolfRunAttackState::UpdateImpact( CCharacter* _pCharacter )
{
	/*Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "WolfRunImpact",	   _pCharacter->GetName() + "_WolfRunImpact",		"", l_Pos);
	SetParticlePosition(_pCharacter, "WolfRunExpandWave",  _pCharacter->GetName() + "_WolfRunExpandWave",	"", l_Pos);
	SetParticlePosition(_pCharacter, "WolfRunAttackRay",   _pCharacter->GetName() + "_WolfRunAttackRay",    "", l_Pos);
	SetParticlePosition(_pCharacter, "WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud",	"", _pCharacter->GetPosition());*/
}

void CWolfRunAttackState::StopImpact( CCharacter* _pCharacter )
{
	//GetParticleGroupInstance("WolfRunAttack","WolfRunAttack")->GetEmitterInstance("...")->StopEjectParticles
	/*GetParticleEmitterInstance("WolfRunImpact",		 _pCharacter->GetName() + "_WolfRunImpact")->StopEjectParticles();
	GetParticleEmitterInstance("WolfRunExpandWave",  _pCharacter->GetName() + "_WolfRunExpandWave")->StopEjectParticles();
	GetParticleEmitterInstance("WolfRunAttackRay",   _pCharacter->GetName() + "_WolfRunAttackRay")->StopEjectParticles();
	GetParticleEmitterInstance("WolfRunAttackCloud", _pCharacter->GetName() + "_WolfRunAttackCloud")->StopEjectParticles();*/
}