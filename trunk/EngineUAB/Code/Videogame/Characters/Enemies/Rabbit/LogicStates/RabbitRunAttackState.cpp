#include "RabbitRunAttackState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Math\Vector3.h"
#include "SoundManager.h"
#include "Core.h"
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
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitAttackState.h"
#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitHitState.h"
#include "RabbitIdleState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitRunAttackAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitRunAnimationState.h"

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
CRabbitRunAttackState::CRabbitRunAttackState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CRabbitRunAttackState")
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_ActionStateCallback	( 0, 1 )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_RUN_ATTACK_STATE);
}

CRabbitRunAttackState::CRabbitRunAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pRabbit				( NULL )
	, m_pAnimationCallback	( NULL )
	, m_ActionStateCallback ( 0, 1 )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),RABBIT_RUN_ATTACK_STATE);
}


CRabbitRunAttackState::~CRabbitRunAttackState(void)
{
	m_pRabbit = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRunAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_pRabbit->SetPlayerHasBeenReached( false );
	m_playerPushed = false;

	/// Esto nos permite hacer el parípé un poco. Situarnos delante la càmara, una simulación de alejarse por cansancio. En este caso no queremos
	// pq hace un desplazamiento que después de este ataque no queremos que haga.
	m_pRabbit->SetToBeTired(false);
	
	// Resetamos el flag que me dice si ya ejecuté partículas de impacto
	m_FirstParticlesHitDone = false;

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_pRabbit->GetProperties()->GetRunAttackSpeed());
	//m_pRabbit->GetSteeringEntity()->SetMaxForce(0.1f);

	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_PlayerInitialPosition		= m_pRabbit->GetPlayer()->GetSteeringEntity()->GetPosition();
	Vect3f l_Position			= m_pRabbit->GetPosition();
	Vect3f l_RelativePosition	= m_PlayerInitialPosition - m_pRabbit->GetPosition();
	Vect3f l_RelativePositionN	= l_RelativePosition.GetNormalized();
	m_InitialDistance			= m_pRabbit->GetDistanceToPlayer();
	if ( m_InitialDistance <= 5 )
	{
		m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 5.f) );
	}
	else
	{
		m_FinalAttackPosition = l_Position + (l_RelativePositionN * ( m_InitialDistance + 0.f) );
	}

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_FinalAttackPosition);
	m_pRabbit->GetBehaviors()->SeparationOn();
	m_pRabbit->GetBehaviors()->CohesionOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();
	
	m_AnimationDuration = m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_RUN_ATTACK_STATE);

	m_pAnimationCallback->Init();
	CORE->GetSoundManager()->PlayEvent(m_pRabbit->GetSpeakerName(), "Play_EFX_Rabbit_Run_Attack"); 

	m_ActionStateCallback.InitAction(0,m_AnimationDuration);
	m_ActionStateCallback.StartAction();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Enter Run Attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif

}

void CRabbitRunAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	UpdateImpact(m_pRabbit);
	
	m_ActionStateCallback.Update(_ElapsedTime);

	if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		// Si el player no estaba ya alcanzado pero ahora se alcanza
		if ( !m_pRabbit->GetPlayerHasBeenReached() && m_pRabbit->IsPlayerReached() )
		{
			m_pRabbit->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Punch3"); 
		}
		
		// Compruebo si la animación ha finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			// Si encontré el player por delante finalizo golpeando
			if ( m_pRabbit->GetPlayerHasBeenReached() )
			{
				m_pRabbit->SetToBeTired(true);

				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pRabbit->GetID(), m_pRabbit->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
					LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitRunAttackState::Execute->Envio mensaje de tocado");
					#if defined _DEBUG
						if( CORE->IsDebugMode() )
						{
							LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitRunAttackState:Execute->Dispatch" );
						}
					#endif
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CRabbitRunAttackState:Execute->El Dispatch es NULL" );
				}
			}
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

			// Volvemos al estado anterior
			m_pRabbit->GetBehaviors()->SeekOff();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pRabbit->FaceTo( m_FinalAttackPosition, _ElapsedTime );
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
				
			// Volvemos a idle
			//m_pRabbit->GetLogicFSM()->RevertToPreviousState();
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());

			return;
		}
		// La animación aun no ha acabado pero está iniciada
		else 
		{
			// Si encuentro el player por delante me lo llevo
			if ( m_pRabbit->GetPlayerHasBeenReached() )
			{
				if ( !m_playerPushed )
				{
					Vect3f l_Vel = m_pRabbit->GetSteeringEntity()->GetVelocity();
					l_Vel.Normalize();
					bool l_isNan  = boost::math::isnan( l_Vel.x );
					if ( l_isNan )
					{
						l_Vel = m_pRabbit->GetSteeringEntity()->GetHeading();
						l_isNan  = boost::math::isnan( l_Vel.x );
						if ( l_isNan )
						{
							l_Vel = Vect3f(0,0,0);
							m_AditionalInfo.Speed = m_pRabbit->GetProperties()->GetRunAttackSpeed();
						}
						else
						{	
							l_Vel *= m_pRabbit->GetProperties()->GetMaxSpeed();
						}
					}
					else
					{
						l_Vel *= m_pRabbit->GetProperties()->GetMaxSpeed();
					}
					m_AditionalInfo.Direccion	= l_Vel;
					m_AditionalInfo.ElapsedTime = _ElapsedTime;
				}
					
				if ( DISPATCH != NULL ) 
				{
					DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pRabbit->GetID(), m_pRabbit->GetPlayer()->GetID(), Msg_Push, &m_AditionalInfo );
					m_playerPushed = true;
				}
				else
				{
					LOGGER->AddNewLog(ELL_ERROR, "CRabbitRunAttackState:Execute->El Dispatch es NULL" );
				}

				UpdateImpact(m_pRabbit);
				if ( m_ActionStateCallback.IsActionInTime(0.57f) && !m_FirstParticlesHitDone )
				{		
					m_FirstParticlesHitDone = true;
					GenerateImpact(m_pRabbit);
				}
			}
			else
			{
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitRunAttackState:Execute->Animation Not finished yet" );
					}
				#endif
			}
			
			if ( m_ActionStateCallback.IsActionInTime(0.4f) )
			{
				m_pRabbit->GetBehaviors()->SeekOn();
				m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_PlayerInitialPosition);
			}

			// No Rotamos al objetivo y pero si movemos. Esto dará sensación de golpear allí donde estava el target cuando inicie el ataque
			//_CCharacter:face_to( self.target_position, _elapsed_time )
			m_pRabbit->FaceTo( m_FinalAttackPosition, _ElapsedTime );
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		}
	}
	// Si l'animación no se ha iniciado
	// Primer estado que se ejecutará. Si está lejos nos acercamos con gran velocidad Corremos rápido hacía el player. 
	// Para ello hay que saber el tiempo de la animación y cuando hay que activarla. 
	else
	{
		m_pRabbit->GetBehaviors()->SeekOff();
		m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_PlayerInitialPosition);
		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitRunAttackState:Execute->Correm al objectiu" );
			}
		#endif
			
		// Ahora azitamos y empezamos la animación de ataque
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAttackAnimationState());
		m_pAnimationCallback->StartAnimation();

		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->FaceTo( m_FinalAttackPosition, _ElapsedTime );
		m_pRabbit->MoveTo2( Vect3f(0,0,0), _ElapsedTime );

		if ( m_pRabbit->GetPlayerHasBeenReached() == false && m_pRabbit->IsPlayerReached() )
		{
			m_pRabbit->SetPlayerHasBeenReached(true);
			CORE->GetSoundManager()->PlayEvent("Play_EFX_RabbitRunAttackCharged"); 
		}
	}
}


void CRabbitRunAttackState::OnExit( CCharacter* _pCharacter )
{
	// Limpiamos animaciones
	/*self.active_animation_name = _CCharacter:get_animation_id("attack_2")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
	self.active_animation_name = _CCharacter:get_animation_id("run")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )*/

	// Quitamos el behaviur
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
				
	// Restauramos la velocidad original
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_pRabbit->GetProperties()->GetMaxSpeed());

	CORE->GetSoundManager()->PlayEvent("Stop_EFX_RabbitsRunAttacks"); 

	// Paramos las partículas
	StopImpact(m_pRabbit);
}

bool CRabbitRunAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
		}

		m_pRabbit->RestLife(1000); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		return true;
	}

	return false;
}


void CRabbitRunAttackState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleGroupInstance("RabbitRunAttack","RabbitRunAttack")->GetEmitterInstance("...");
	GetParticleEmitterInstance("RabbitRunImpact1",	   _pCharacter->GetName() + "_RabbitRunImpact1")->EjectParticles();
	GetParticleEmitterInstance("RabbitRunImpact2",	   _pCharacter->GetName() + "_RabbitRunImpact2")->EjectParticles();
	GetParticleEmitterInstance("RabbitRunImpact3",	   _pCharacter->GetName() + "_RabbitRunImpact3")->EjectParticles();
	GetParticleEmitterInstance("RabbitRunExpandWave",  _pCharacter->GetName() + "_RabbitRunExpandWave")->EjectParticles();
	//GetParticleEmitterInstance("RabbitRunAttackRay",   _pCharacter->GetName() + "_RabbitRunAttackRay")->EjectParticles();
	//GetParticleEmitterInstance("RabbitRunAttackCloud", _pCharacter->GetName() + "_RabbitRunAttackCloud")->EjectParticles();
}

void CRabbitRunAttackState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPlayer()->GetPosition();
	l_Pos.y += _pCharacter->GetPlayer()->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "RabbitRunImpact1",	     _pCharacter->GetName() + "_RabbitRunImpact1",		"", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitRunImpact2",	     _pCharacter->GetName() + "_RabbitRunImpact2",		"", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitRunImpact3",	     _pCharacter->GetName() + "_RabbitRunImpact3",		"", l_Pos);
	SetParticlePosition(_pCharacter, "RabbitRunExpandWave",		 _pCharacter->GetName() + "_RabbitRunExpandWave",	"", l_Pos);
	//SetParticlePosition(_pCharacter, "RabbitRunAttackRay",   _pCharacter->GetName() + "_RabbitRunAttackRay",    "", l_Pos);
	//SetParticlePosition(_pCharacter, "RabbitRunAttackCloud", _pCharacter->GetName() + "_RabbitRunAttackCloud",	"", _pCharacter->GetPosition());
}

void CRabbitRunAttackState::StopImpact( CCharacter* _pCharacter )
{
	//GetParticleGroupInstance("RabbitRunAttack","RabbitRunAttack")->GetEmitterInstance("...")->StopEjectParticles
	GetParticleEmitterInstance("RabbitRunImpact1", _pCharacter->GetName() + "_RabbitRunImpact1")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitRunImpact2", _pCharacter->GetName() + "_RabbitRunImpact2")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitRunImpact3", _pCharacter->GetName() + "_RabbitRunImpact3")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitRunExpandWave",  _pCharacter->GetName() + "_RabbitRunExpandWave")->StopEjectParticles();
	//GetParticleEmitterInstance("RabbitRunAttackRay",   _pCharacter->GetName() + "_RabbitRunAttackRay")->StopEjectParticles();
	//GetParticleEmitterInstance("RabbitRunAttackCloud", _pCharacter->GetName() + "_RabbitRunAttackCloud")->StopEjectParticles();
}