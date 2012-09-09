#include "RabbitPreparedToAttackState.h"
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

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitAttackState.h"
#include "RabbitHitState.h"
#include "RabbitIdleState.h"

#include "RabbitRunAnimationState.h"
#include "RabbitHitAnimationState.h"
#include "RabbitWalkAnimationState.h"
#include "RabbitIdleAnimationState.h"

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
CRabbitPreparedToAttackState::CRabbitPreparedToAttackState( void )
	: CState	("CRabbitPreparedToAttackState")
	, m_pRabbit	( NULL )
{
}

CRabbitPreparedToAttackState::CRabbitPreparedToAttackState( const std::string &_Name )
	: CState		(_Name)
	, m_pRabbit		( NULL )
{
}


CRabbitPreparedToAttackState::~CRabbitPreparedToAttackState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitPreparedToAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOn();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOn();
	m_pRabbit->GetBehaviors()->SeparationOn();
}

void CRabbitPreparedToAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		//LOGGER->AddNewLog(ELL_INFORMATION, "Enemy %s preparing to attack...", m_pRabbit->GetName().c_str() );
	}
	#endif


	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pRabbit->IsPlayerAtacable() ) 
	{
		// Reseteamos la velocidad del enemigo
		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetAttackState() );
		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared-Atacable");
			}
		#endif
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pRabbit->IsEnemyPreparedToAttack() ) 
	{
		#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared to attack");
		}
		#endif


		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido (el que realmente ataca ya que solo ataca 1)
		if ( m_pRabbit->GetReadyToAttack() ) 
		{
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Ready to attack");
				}
			#endif

		//	if ( IsCollisionedWithSomething() )
		//	{
				// Este enemigo puede atacar. Ahora miro si está dentro del angulo de vision pero no es el elegido para atacar. Por tanto, vamos hacia el player para tener opciones de ser
				// el elegido para atacar
				float l_Angle = 22.f;			//math.pi/15		// 12 graus de fustrum
				m_pRabbit->GoInToFrustrum(l_Angle, _ElapsedTime);
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());		// dudo de si uno u otro. Faltan pasos laterales...
				//m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
				/*#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared-Walk");
					}
				#endif*/
			//}	
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

			//m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
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
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetPursuitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
	}
}


void CRabbitPreparedToAttackState::OnExit( CCharacter* _Character )
{
	
	m_pRabbit->GetBehaviors()->SeekOff();
	m_pRabbit->GetBehaviors()->PursuitOff();
	m_pRabbit->GetBehaviors()->CollisionAvoidanceOff();
	m_pRabbit->GetBehaviors()->ObstacleWallAvoidanceOff();
	m_pRabbit->GetBehaviors()->SeparationOff();
}

bool CRabbitPreparedToAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
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

		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_Character);
		}

		m_pRabbit->RestLife(1000); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		return true;
	}

	return false;
}