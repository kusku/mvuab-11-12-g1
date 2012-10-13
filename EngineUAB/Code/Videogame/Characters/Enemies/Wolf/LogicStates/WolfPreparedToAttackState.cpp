#include "WolfPreparedToAttackState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Wolf\Wolf.h"
#include "Characters\Character.h"

#include "WolfPursuitState.h"
#include "WolfAttackState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfWalkAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

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
CWolfPreparedToAttackState::CWolfPreparedToAttackState( CCharacter* _pCharacter )
	: CState								( _pCharacter, "CWolfPreparedToAttackState")
	, m_pWolf								( NULL )
	, m_IsPositionAfterHitPlayerAssigned	( false )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}

CWolfPreparedToAttackState::CWolfPreparedToAttackState( CCharacter* _pCharacter, const std::string &_Name )
	: CState								(_pCharacter, _Name)
	, m_pWolf								( NULL )
	, m_IsPositionAfterHitPlayerAssigned	( false )
{
	if ( _pCharacter != NULL ) 
	{
		m_AngleRangeFromCamara = CORE->GetSteeringBehaviourSettingsManager()->GetCamaraRangeAngleForAttack();
	}
}


CWolfPreparedToAttackState::~CWolfPreparedToAttackState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfPreparedToAttackState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetBehaviors()->SeparationOn();
	m_pWolf->GetBehaviors()->CohesionOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();
	
#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Prepared to attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfPreparedToAttackState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	// 0) Caso en que alcanzé al player y por tanto vamos a un punto de inicio de ataque. Así dejo que el player se reponga
	if ( m_pWolf->GetPlayerHasBeenReached() && m_pWolf->GetIsTired() )
	{
		// Si no ser donde tengo que ir...
		if ( !m_IsPositionAfterHitPlayerAssigned )
		{
			m_PositionReachedAfterHitPlayer = m_pWolf->GetPointInsideCameraFrustum(m_AngleRangeFromCamara);
			m_IsPositionAfterHitPlayerAssigned	= true;
		}

		// Mira si alcanzamos la posición. Reseteamos indicando que este enemigo ya ha realizado las tareas postimpacto 
		Vect2f l_Pos1 = Vect2f(m_pWolf->GetPosition().x, m_pWolf->GetPosition().z);
		Vect2f l_Pos2 = Vect2f(m_PositionReachedAfterHitPlayer.x, m_PositionReachedAfterHitPlayer.z);
		float l_DistanceToCameraPoint = l_Pos1.Distance(l_Pos2);
		//float l_DistanceToCameraPoint = m_pWolf->GetPosition().Distance(m_PositionReachedAfterHitPlayer);
		if ( l_DistanceToCameraPoint <= 3.01f )
		{
			m_IsPositionAfterHitPlayerAssigned = false;		// Reiniciamos el flag para la pròxima vez
			m_pWolf->SetPlayerHasBeenReached(false);		// Reiniciamos el flag de player alcanzado
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			return;
		}
		else
		{
			//float l_DistanceToPlayer = m_pWolf->GetPosition().Distance(m_pWolf->GetPlayer()->GetPosition());
			// Evitamos que vayamos a un punto donde se había calculado inicialmente pero que ahora és demasido lejos del player
			// Esto passa si el player lo mandamos tant y tant lejos que luego es excesivo ir a ese punto y mejor recalcularlo
			/*if ( l_DistanceToCameraPoint < l_DistanceToPlayer && l_DistanceToCameraPoint > m_pWolf->GetProperties()->GetPreparedAttackDistance())
			{
				m_IsPositionAfterHitPlayerAssigned = false;	
				m_pWolf->SetPlayerHasBeenReached(true);
			}
			else
			{*/
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
				m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_PositionReachedAfterHitPlayer);
				m_pWolf->GetBehaviors()->SeekOn();
				m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
				m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
				LOGGER->AddNewLog(ELL_INFORMATION, "CWolfPreparedToAttackState::Execute -> %s peguó al player y ahora vuelve a una posición inicial de ataque", m_pWolf->GetName().c_str());
			//}
		}
	}

	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pWolf->IsPlayerAtacable() ) 
	{
		m_pWolf->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetAttackState() );

		// Reseteamos la velocidad del enemigo
		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );
		m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );

		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				LOGGER->AddNewLog(ELL_INFORMATION,"CWolfPreparedToAttackState::Execute->Change to Attack State");
			}
		#endif
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pWolf->IsEnemyPreparedToAttack() ) 
	{
		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido (el que realmente ataca ya que solo ataca 1)
		if ( m_pWolf->GetAvalaibleToAttack() ) 
		{
			// Este enemigo podria atacar pero no es el seleccionado. Ahora miro si está dentro del angulo de vision y si no lo está lo metemos para que el player pueda verlo
			if ( !m_pWolf->IsEnemyIntoCameraFrustum( m_AngleRangeFromCamara, _ElapsedTime ) )
			{
				m_pWolf->GoIntoCameraFrustum(m_AngleRangeFromCamara, _ElapsedTime);
				return;
			}

			//Vect3f l_NewAttackPosition = GetPositionToAttack();
			m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
			m_pWolf->GetBehaviors()->SeekOn();
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);

			//m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetWalkAnimationState());		// dudo de si uno u otro. Faltan pasos laterales...
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					//LOGGER->AddNewLog(ELL_INFORMATION,"CWolfPreparedToAttackState::Execute->Prepared-Walk");
				}
			#endif
		}
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player. 
		// Exite un total de enemigos a atacar. El resto se quedan en idle
		else
		{
			//m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetWalkAnimationState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					LOGGER->AddNewLog(ELL_INFORMATION,"CWolfPreparedToAttackState::Execute->Not Ready-Too far");
				}
			#endif
		}
	}
	else
	{
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetPursuitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
	}
}


void CWolfPreparedToAttackState::OnExit( CCharacter* _pCharacter )
{
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();
	m_pWolf->GetBehaviors()->SeparationOff();
}

bool CWolfPreparedToAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	return m_pWolf->CallHitState(_pCharacter, _Telegram);
}