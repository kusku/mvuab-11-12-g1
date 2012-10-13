#include "DeerPreparedToAttackState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Deer\Deer.h"

#include "DeerPursuitState.h"
#include "DeerAttackState.h"
#include "DeerHitState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerRunAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerWalkAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"

#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerPreparedToAttackState::CDeerPreparedToAttackState( CCharacter* _pCharacter )
	: CState								( _pCharacter, "CDeerPreparedToAttackState")
	, m_pDeer								( NULL )
	, m_IsPositionAssignedAfterHitPlayer	( false )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}

CDeerPreparedToAttackState::CDeerPreparedToAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState								(_pCharacter, _Name)
	, m_pDeer								( NULL )
	, m_IsPositionAssignedAfterHitPlayer	( false )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}


CDeerPreparedToAttackState::~CDeerPreparedToAttackState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerPreparedToAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_pDeer->GetBehaviors()->SeparationOn();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOn();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOn();

	m_IsPositionAssignedAfterHitPlayer = false;

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Prepared to attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerPreparedToAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// 0) Caso en que alcanzé al player y por tanto vamos a un punto de inicio de ataque. Así dejo que el player se reponga
	if ( m_pDeer->GetPlayerHasBeenReached() && m_pDeer->GetIsTired() )
	{
		// Si no ser donde tengo que ir...
		if ( !m_IsPositionAssignedAfterHitPlayer )
		{
			m_PositionReachedAfterHitPlayer = m_pDeer->GetPointInsideCameraFrustum(m_AngleRangeFromCamara);
			m_IsPositionAssignedAfterHitPlayer	= true;
		}

		// Mira si alcanzamos la posición. Reseteamos indicando que este enemigo ya ha realizado las tareas postimpacto 
		Vect2f l_Pos1 = Vect2f(m_pDeer->GetPosition().x, m_pDeer->GetPosition().z);
		Vect2f l_Pos2 = Vect2f(m_PositionReachedAfterHitPlayer.x, m_PositionReachedAfterHitPlayer.z);
		float l_DistanceToCameraPoint = l_Pos1.Distance(l_Pos2);
		//float l_DistanceToCameraPoint = m_pDeer->GetPosition().Distance(m_PositionReachedAfterHitPlayer);
		if ( l_DistanceToCameraPoint <= 2.3f )
		{
			m_IsPositionAssignedAfterHitPlayer = false;		// Reiniciamos el flag para la pròxima vez
			m_pDeer->SetPlayerHasBeenReached(false);		// Reiniciamos el flag de player alcanzado
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
			m_pDeer->SetToBeTired(false); 
		}
		else
		{
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
			m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_PositionReachedAfterHitPlayer);
			m_pDeer->GetBehaviors()->SeekOn();
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			LOGGER->AddNewLog(ELL_INFORMATION, "CDeerPreparedToAttackState::Execute -> %s peguó al player y ahora vuelve a una posición inicial de ataque", m_pDeer->GetName().c_str());
		}
		return;
	}

	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pDeer->IsPlayerAtacable() ) 
	{
		m_pDeer->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetAttackState() );

		// Reseteamos la velocidad del enemigo
		m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
		m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION,"CDeerPreparedToAttackState::Execute->Change to Attack State");
			}
		#endif
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pDeer->IsEnemyPreparedToAttack() ) 
	{
		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido (el que realmente ataca ya que solo ataca 1)
		if ( m_pDeer->GetAvalaibleToAttack() ) 
		{
			// Este enemigo podria atacar pero no es el seleccionado. Ahora miro si está dentro del angulo de vision y si no lo está lo metemos para que el player pueda verlo
			if ( !m_pDeer->IsEnemyIntoCameraFrustum( m_AngleRangeFromCamara, _ElapsedTime ) )
			{
				m_pDeer->GoIntoCameraFrustum(m_AngleRangeFromCamara, _ElapsedTime);
				return;
			}

			//Vect3f l_NewAttackPosition = GetPositionToAttack();
			m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
			m_pDeer->GetBehaviors()->SeekOn();
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			//m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetWalkAnimationState());		// dudo de si uno u otro. Faltan pasos laterales...
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					//LOGGER->AddNewLog(ELL_INFORMATION,"CDeerPreparedToAttackState::Execute->Prepared-Walk");
				}
			#endif
		}
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player. 
		// Exite un total de enemigos a atacar. El resto se quedan en idle
		else
		{
			//m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetWalkAnimationState());
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CDeerPreparedToAttackState::Execute->Not Ready-Too far");
				}
			#endif
		}
	}
	else
	{
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetPursuitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
	}
}


void CDeerPreparedToAttackState::OnExit( CCharacter* _pCharacter )
{
	m_pDeer->GetBehaviors()->SeekOff();
	m_pDeer->GetBehaviors()->PursuitOff();
	m_pDeer->GetBehaviors()->CollisionAvoidanceOff();
	m_pDeer->GetBehaviors()->ObstacleWallAvoidanceOff();
	m_pDeer->GetBehaviors()->SeparationOff();
}

bool CDeerPreparedToAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	return m_pDeer->CallHitState(m_pDeer, _Telegram);
}