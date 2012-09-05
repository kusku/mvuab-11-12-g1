#include "WolfRunAttackState.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "StatesMachine\MessageDispatcher.h"
#include "Math\Vector3.h"

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfRunAttackState::CWolfRunAttackState( void )
	: CState				("CWolfRunAttackState")
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RUN_ATTACK_STATE);
}

CWolfRunAttackState::CWolfRunAttackState( const std::string &_Name )
	: CState				(_Name)
	, m_pWolf				( NULL )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(RUN_ATTACK_STATE);
}


CWolfRunAttackState::~CWolfRunAttackState(void)
{
	m_pWolf = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfRunAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pWolf->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pWolf->GetSteeringEntity()->GetMass();
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(1);
	m_pWolf->GetSteeringEntity()->SetMass(0.00500f);

	// Activo el seek a saco a una posició en el momento de inicio de ataque
	m_pWolf->GetBehaviors()->SeekOn();
	m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
		
	// Almacenamos la distancia actual para saber si luego nos hemos pasado
	m_CurrentDistance = m_pWolf->GetDistanceToPlayer();
}

void CWolfRunAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	// Compruebo si la animación a finalizado
	if ( m_pAnimationCallback->IsAnimationFinished() ) 
	{
		if ( DISPATCH != NULL ) 
		{
			DISPATCH->DispatchStateMessage(SEND_MSG_IMMEDIATELY, m_pWolf->GetID(), m_pWolf->GetPlayer()->GetID(), Msg_Attack, NO_ADDITIONAL_INFO );
			m_pAnimationCallback->Init();
		}
		else
		{
			LOGGER->AddNewLog(ELL_ERROR, "CWolfRunAttackState:Execute->El Dispatch es NULL" );
		}
					
		// Incrementamos el nº de ataques hechos --> si llega a un total estará cansado
		m_pWolf->SetHitsDone(m_pWolf->GetHitsDone() + 1);

		// Volvemos al estado anterior
		m_pWolf->GetBehaviors()->SeekOff();
		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		m_pWolf->GetLogicFSM()->RevertToPreviousState();
	}
	else 
	{
		float l_Distance = m_pWolf->GetDistanceToPlayer();
		
		// Pararemos de correr si estamos en el momento de impacto o que la distancia al player no sea mayor que la inicial ya que indicará 
		// que el ataque seguramente falló y así evitamos que exista un pequeño retroceso de volver hacia el player
		if ( m_pWolf->IsPlayerInsideImpactDistance() || ( l_Distance > m_CurrentDistance ) ) 
		{
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pWolf->FaceTo( m_pWolf->GetSteeringEntity()->GetPosition(), _ElapsedTime );
		}
			
		// Rotamos al objetivo y movemos
		//_CCharacter:face_to( self.target_position, _elapsed_time )
		m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
	}	
}


void CWolfRunAttackState::OnExit( CCharacter* _Character )
{
	// Limpiamos animaciones
	/*self.active_animation_name = _CCharacter:get_animation_id("attack_2")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )
		
	self.active_animation_name = _CCharacter:get_animation_id("run")
	_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0.3 )*/

	// Quitamos el behaviur
	m_pWolf->GetBehaviors()->SeekOff();
					
	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pWolf->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CWolfRunAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}

	return false;
}