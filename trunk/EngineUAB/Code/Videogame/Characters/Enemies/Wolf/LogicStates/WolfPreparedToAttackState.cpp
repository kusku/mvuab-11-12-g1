#include "WolfPreparedToAttackState.h"
#include "Utils\BoostRandomHelper.h"

#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfAttackState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfWalkAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

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
}

void CWolfPreparedToAttackState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

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
		// Si el player puede atacar porque és uno de los más cercanos pero aun no és el elegido
		if ( m_pWolf->GetReadyToAttack() ) 
		{
			// Este enemigo puede atacar. Ahora miro si está dentro del angulo de vision pero no es el elegido para atacar. Por tanto, vamos hacia el player para tener opciones de ser
			// el elegido para atacar
			float l_Angle = 15.f;			//math.pi/15		// 12 graus de fustrum
			m_pWolf->GoInToFrustrum(l_Angle, _ElapsedTime);
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetWalkAnimationState());
		}
		// Si el enemigo no está listo para atacar ya que està más lejos que los que deben atacar. Reseteamos velocidad y encaramos al player
		else
		{
			//m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetWalkAnimationState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
			m_pWolf->MoveTo2(m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
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
}

bool CWolfPreparedToAttackState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		//CState<CCharacter> * l_State = dynamic_cast<CState<CCharacter>*> (m_pWolf->GetHitAnimationState());
		_Character->GetLogicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}

	return false;
}