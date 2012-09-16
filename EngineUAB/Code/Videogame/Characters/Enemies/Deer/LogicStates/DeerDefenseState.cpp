#include "DeerDefenseState.h"
#include "Utils\BoostRandomHelper.h"
#include "Math\MathUtils.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Deer\Deer.h"

#include "DeerPursuitState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerAttackState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerDefenseAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Flee.h"
#include "Steering Behaviors\Seek.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerDefenseState::CDeerDefenseState( void )
	: CState			("CDeerDefenseState")
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pDeer			( NULL )
	, m_HitBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}

CDeerDefenseState::CDeerDefenseState( const std::string &_Name )
	: CState			(_Name)
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pDeer			( NULL )
	, m_HitBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}


CDeerDefenseState::~CDeerDefenseState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerDefenseState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	m_ActionTime.StartAction();

	// Me dice si bloqueo
	m_HitBlocked = 0;

	// Me dice la distancia que recorro cuando pega el player y bloqueo hacia atras
	m_HitDistance = m_pDeer->GetProperties()->GetImpactDistance() + 2;
	
	// Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
	m_TotalHitBlocked = BoostRandomHelper::GetInt(1, 4);  		
	// print_logger (1, "nº hits totals x blojar"..self.total_hit_blocked )
		
	// me dice el nº de veces que el player me pega mientras bloqueo
	m_HitBlockedCount = 0;
		
	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pDeer->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pDeer->GetSteeringEntity()->GetMass();
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(1);
	m_pDeer->GetSteeringEntity()->SetMass(0.00500f);

	
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Attack";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerDefenseState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );

	if ( m_HitBlocked ) 
	{
		float l_Distance = m_pDeer->GetDistanceToPlayer();
		// Si aun no he hecho el retroceso lo sigo moviendo
		//if ( l_Distance <= m_HitDistance ) 
		//{
		//	//m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
		//}
		// Si llego al destino paro el retroceso
		//else
		//{
		if ( l_Distance > m_HitDistance ) 
		{
			m_pDeer->GetBehaviors()->SeekOn();
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			//m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_HitBlocked = false;
		} 
		//m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
		m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
	}	
	else 
	{
		// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( m_pDeer->IsPlayerAtacable() )
		{
			// Si asoleixo el total de hits bloquejats retorno al anterior estat
			if ( m_HitBlockedCount == m_TotalHitBlocked ) 
			{
				//print_logger (1, "retorno")
				m_pDeer->SetReceivedHitsXMinut(0);
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				return;
			}
			
			// Solo hago la acción si estoy dentro de la distancia de impacto
			float l_Distance = m_pDeer->GetDistanceToPlayer();
			if ( l_Distance <= ( m_pDeer->GetProperties()->GetImpactDistance() * 2 ) ) 
			{
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetDefenseAnimationState());
			}
			else 
			{
				m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}		

			if ( m_ActionTime.IsActionFinished() ) 
			{
				// nos volvemos
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());		
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());		
			}
			else 
			{
				// Incrementamos el tiempo que llevamos en este estado
				m_ActionTime.Update(_ElapsedTime);
			}
			//m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		}
		// Si el player NO es atacable lo volvemos a preparar o a perseguir
		else 
		{
			m_pDeer->SetReceivedHitsXMinut(0);
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetAttackState());		
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());		
		}
	}

}

void CDeerDefenseState::OnExit( CCharacter* _Character )
{
	m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pDeer->GetBehaviors()->SeekOff();

	// Restauramos la velocidad original
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pDeer->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CDeerDefenseState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// Solo me pongo en hit si realmente he finalizado el estado de ataque
			
		// Me dice que acabo de bloquear un golpe
		m_HitBlocked = true;

		// _CCharacter.behaviors:flee_on()
		m_pDeer->GetBehaviors()->SeekOn();
		Vect3f l_Front = m_pDeer->GetSteeringEntity()->GetFront();
		l_Front.Normalize();
		l_Front = l_Front.RotateY(mathUtils::PiTimes(1.f));
			
		//m_pDeer->GetBehaviors()->GetFlee()->SetTarget(m_pDeer->GetPlayer()->GetPosition());
		// l_target = Vect3f(l_front.x,l_front.y,l_front.z):normalize(1)
		// l_target = l_target * 2
		// _CCharacter.behaviors.flee.target = l_target
			
		l_Front = m_pDeer->GetSteeringEntity()->GetPosition() + l_Front * m_HitDistance;
		// _CCharacter.behaviors.flee.target = l_front
		m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pDeer->GetBehaviors()->GetSeek()->SetTarget(l_Front);
					
		// _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )

		// Cuento el nº de hits que lleva mientras bloqueo
		m_HitBlockedCount += 1;
		//print_logger (1, "self.hit_blocked_count : "..self.hit_blocked_count)
		return true;
	}

	return false;
}