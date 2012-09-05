#include "RabbitPreparedToAttackState.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitAttackState.h"

#include "RabbitRunAnimationState.h"
#include "RabbitHitAnimationState.h"
#include "RabbitWalkAnimationState.h"
#include "RabbitIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfPreparedToAttackState::CWolfPreparedToAttackState( void )
	: CState	("CWolfPreparedToAttackState")
	, m_pRabbit	( NULL )
{
}

CWolfPreparedToAttackState::CWolfPreparedToAttackState( const std::string &_Name )
	: CState		(_Name)
	, m_pRabbit		( NULL )
{
}


CWolfPreparedToAttackState::~CWolfPreparedToAttackState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfPreparedToAttackState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}
}

void CWolfPreparedToAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	// 1) Caso en que ataco al player. Si está focalizado y suficientemente cerca de atacar lo hace independientemente del angulo de visión del player
	if ( m_pRabbit->IsPlayerAtacable() ) 
	{
		// Reseteamos la velocidad del enemigo
		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->SetHitsDone(2);		// Esto permite hacer una pausa al entrar en el estado de ataque antes de atacar por obligar estar fatigado y permitir ver al player qué va a hacer el enemigo
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetAttackState() );
	}
	
	// 2) Si el player NO es atacable pero casi nos aproximamos. Buscamos el hueco que no col·lisione con nada.
	else if ( m_pRabbit->IsEnemyPreparedToAttack() ) 
	{
		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido
		if ( m_pRabbit->GetReadyToAttack() ) 
		{
			// Este enemigo puede atacar. Ahora miro si está dentro del angulo de vision pero no es el elegido para atacar. Por tanto, vamos hacia el player para tener opciones de ser
			// el elegido para atacar
			float l_Angle = 15.f;			//math.pi/15		// 12 graus de fustrum
			m_pRabbit->GoInToFrustrum(l_Angle, _ElapsedTime);
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());
		}
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player
		else
		{
			//m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetWalkAnimationState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pRabbit->MoveTo2(m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
		}
	}
	else
	{
		m_pRabbit->GetLogicFSM()->ChangeState( m_pRabbit->GetPursuitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetRunAnimationState());
	}
}


void CWolfPreparedToAttackState::OnExit( CCharacter* _Character )
{
}

bool CWolfPreparedToAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pRabbit->GetHitAnimationState());
		_Character->GetLogicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}

	return false;
}