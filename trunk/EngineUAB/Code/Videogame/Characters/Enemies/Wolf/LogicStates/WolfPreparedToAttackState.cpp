#include "WolfPreparedToAttackState.h"
#include "Utils\BoostRandomHelper.h"
#include "Utils\Random.h"
#include "GameProcess.h"
#include "Logger\Logger.h"
#include "Base.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfAttackState.h"
#include "WolfHitState.h"
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfWalkAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\CollisionAvoidance.h"
#include "Steering Behaviors\ObstacleWallAvoidance.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfPreparedToAttackState::CWolfPreparedToAttackState( void )
	: CState	("CWolfPreparedToAttackState")
	, m_pWolf	( NULL )
{
}

CWolfPreparedToAttackState::CWolfPreparedToAttackState( const std::string &_Name )
	: CState		(_Name)
	, m_pWolf		( NULL )
{
}


CWolfPreparedToAttackState::~CWolfPreparedToAttackState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfPreparedToAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOn();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOn();
	m_pWolf->GetBehaviors()->SeparationOn();
}

void CWolfPreparedToAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s preparing to attack...", m_pWolf->GetName().c_str() );
	}
	#endif

	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pWolf->IsPlayerAtacable() ) 
	{
		// Reseteamos la velocidad del enemigo
		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetAttackState() );
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pWolf->IsEnemyPreparedToAttack() ) 
	{
		#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared to attack");
		}
		#endif

		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido
		if ( m_pWolf->GetReadyToAttack() ) 
		{
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Ready to attack");
				}
			#endif

			// Este enemigo puede atacar. Ahora miro si está dentro del angulo de vision pero no es el elegido para atacar. Por tanto, vamos hacia el player para tener opciones de ser
			// el elegido para atacar
			float l_Angle = 22.f;			//math.pi/15		// 12 graus de fustrum
			m_pWolf->GoInTofrustum(l_Angle, _ElapsedTime);
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetWalkAnimationState());
		}
		
		// Hay un nº límite de enemigos a atacar. Esta propiedad Ready nos indica si éste enemigo va a actuar. 
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player
		else
		{
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Ready to attack");
				}
			#endif

			//m_pWolf->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Not Ready-Too far");
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


void CWolfPreparedToAttackState::OnExit( CCharacter* _Character )
{
	m_pWolf->GetBehaviors()->SeekOff();
	m_pWolf->GetBehaviors()->PursuitOff();
	m_pWolf->GetBehaviors()->CollisionAvoidanceOff();
	m_pWolf->GetBehaviors()->ObstacleWallAvoidanceOff();
	m_pWolf->GetBehaviors()->SeparationOff();
}

bool CWolfPreparedToAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	{
		// TODO!!
		/*CRandom	l_Randomize;

		CCharacter *l_pPlayer	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Telegram.Sender);
		float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pPlayer->GetProperties()->GetStrong() / 2), (float)l_pPlayer->GetProperties()->GetStrong());
		float l_fPainToHit		= l_pPlayer->GetProperties()->GetStrong() * 0.95f;*/

		/*if( l_fReceivedPain >= l_fPainToHit )
		{
			m_pRabbit->RestLife(10000); 
		}*/

		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_Character);
		}

		m_pWolf->RestLife(1000); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		return true;
	}

	return false;
}