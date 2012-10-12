#include "RabbitPreparedToAttackState.h"
#include "Utils\BoostRandomHelper.h"
#include "Utils\Random.h"
#include "Math\Vector2.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Base.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitAttackState.h"
#include "RabbitHitState.h"
#include "RabbitIdleState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitRunAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitWalkAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitPreparedToAttackState::CRabbitPreparedToAttackState( CCharacter* _pCharacter )
	: CState								( _pCharacter, "CRabbitPreparedToAttackState")
	, m_pRabbit								( NULL )
	, m_IsPositionAssignedAfterHitPlayer	( false )
{
}

CRabbitPreparedToAttackState::CRabbitPreparedToAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState								(_pCharacter, _Name)
	, m_pRabbit								( NULL )
	, m_IsPositionAssignedAfterHitPlayer	( false )
{
}


CRabbitPreparedToAttackState::~CRabbitPreparedToAttackState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitPreparedToAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();
	m_pRabbit->GetBehaviors()->SeparationOn();

	m_IsPositionAssignedAfterHitPlayer = false;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Prepared to attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif
}

void CRabbitPreparedToAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	// 0) Caso en que alcanzé al player y por tanto vamos a un punto de inicio de ataque. Así dejo que el player se reponga
	if ( m_pRabbit->GetPlayerHasBeenReached() && m_pRabbit->GetIsTired() )
	{
		// Si no ser donde tengo que ir...
		if ( !m_IsPositionAssignedAfterHitPlayer )
		{
			m_PositionReachedAfterHitPlayer = m_pRabbit->GetPointInsideCameraFrustum();
			m_IsPositionAssignedAfterHitPlayer	= true;
		}

		// Mira si alcanzamos la posición. Reseteamos indicando que este enemigo ya ha realizado las tareas postimpacto 
		Vect2f l_Pos1 = Vect2f(m_pRabbit->GetPosition().x, m_pRabbit->GetPosition().z);
		Vect2f l_Pos2 = Vect2f(m_PositionReachedAfterHitPlayer.x, m_PositionReachedAfterHitPlayer.z);
		float l_DistanceToCameraPoint = l_Pos1.Distance(l_Pos2);
		//float l_DistanceToCameraPoint = m_pRabbit->GetPosition().Distance(m_PositionReachedAfterHitPlayer);
		if ( l_DistanceToCameraPoint <= 1.3f )
		{
			m_IsPositionAssignedAfterHitPlayer = false;		// Reiniciamos el flag para la pròxima vez
			m_pRabbit->SetPlayerHasBeenReached(false);		// Reiniciamos el flag de player alcanzado
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			m_pRabbit->SetToBeTired(false); 
		}
		else
		{
			m_pRabbit->GetBehaviors()->SeekOn();
			m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_PositionReachedAfterHitPlayer);
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			LOGGER->AddNewLog(ELL_INFORMATION, "CRabbitPreparedToAttackState::Execute -> %s peguó al player y ahora vuelve a una posición inicial de ataque", m_pRabbit->GetName().c_str());
		}
		return;
	}
	
	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pRabbit->IsPlayerAtacable() ) 
	{
		// Reseteamos la velocidad del enemigo
		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetAttackState() );

		// Reseteamos la velocidad del enemigo
		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
		m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitPreparedToAttackState::Execute->Change to Attack State");
			}
		#endif
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pRabbit->IsEnemyPreparedToAttack() ) 
	{
		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido (el que realmente ataca ya que solo ataca 1)
		if ( m_pRabbit->GetAvalaibleToAttack() ) 
		{
			// Este enemigo podria atacar pero no es el seleccionado. Ahora miro si está dentro del angulo de vision y si no lo está lo metemos para que el player pueda verlo
			float l_Angle = 60.f;			//math.pi/15 == 12 graus de fustrum
			if ( !m_pRabbit->IsEnemyIntoCameraFrustum( l_Angle, _ElapsedTime ) )
			{
				m_pRabbit->GoIntoCameraFrustum(l_Angle, _ElapsedTime);
				return;
			}

			//Vect3f l_NewAttackPosition = GetPositionToAttack();
			m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
			m_pRabbit->GetBehaviors()->SeekOn();
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			//m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());		// dudo de si uno u otro. Faltan pasos laterales...
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					//LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitPreparedToAttackState::Execute->Prepared-Walk");
				}
			#endif
		}
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player. 
		// Exite un total de enemigos a atacar. El resto se quedan en idle
		else
		{
			//m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CRabbitPreparedToAttackState::Execute->Not Ready-Too far");
				}
			#endif
		}
	}
	else
	{
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetPursuitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
	}
}


void CRabbitPreparedToAttackState::OnExit( CCharacter* _pCharacter )
{
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
	m_pRabbit->GetBehaviors()->SeparationOff();
}

bool CRabbitPreparedToAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	return m_pRabbit->CallHitState(m_pRabbit, _Telegram);
}