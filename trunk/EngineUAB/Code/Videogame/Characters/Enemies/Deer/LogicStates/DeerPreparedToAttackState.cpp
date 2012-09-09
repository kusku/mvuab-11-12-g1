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

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerPreparedToAttackState::CDeerPreparedToAttackState( void )
	: CState	("CDeerPreparedToAttackState")
	, m_pDeer	( NULL )
{
}

CDeerPreparedToAttackState::CDeerPreparedToAttackState( const std::string &_Name )
	: CState		(_Name)
	, m_pDeer		( NULL )
{
}


CDeerPreparedToAttackState::~CDeerPreparedToAttackState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerPreparedToAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared to attack");
		}
	#endif
}

void CDeerPreparedToAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pDeer->IsPlayerAtacable() ) 
	{
		// Reseteamos la velocidad del enemigo
		m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pDeer->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		//m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetAttackState() );
		#if defined _DEBUG
			if( CORE->IsDebugMode() )
			{
				CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared-Atacable");
			}
		#endif
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pDeer->IsEnemyPreparedToAttack() ) 
	{
		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido (el que realmente ataca ya que solo ataca 1)
		if ( m_pDeer->GetReadyToAttack() ) 
		{
			// Este enemigo puede atacar. Ahora miro si está dentro del angulo de vision pero no es el elegido para atacar. Por tanto, vamos hacia el player para tener opciones de ser
			// el elegido para atacar
			float l_Angle = 15.f;			//math.pi/15		// 12 graus de fustrum
			m_pDeer->GoInToFrustrum(l_Angle, _ElapsedTime);
			//m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetWalkAnimationState());		// dudo de si uno u otro. Faltan pasos laterales...
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
			#if defined _DEBUG
				if( CORE->IsDebugMode() )
				{
					CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Prepared-Walk");
				}
			#endif
		}
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player
		else
		{
			//m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetWalkAnimationState());
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
			m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pDeer->MoveTo2(m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
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
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetPursuitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
	}
}


void CDeerPreparedToAttackState::OnExit( CCharacter* _Character )
{
}

bool CDeerPreparedToAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		//m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}