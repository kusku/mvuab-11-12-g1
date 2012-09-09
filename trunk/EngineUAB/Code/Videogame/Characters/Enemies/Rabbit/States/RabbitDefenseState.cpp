#include "RabbitDefenseState.h"
#include "Utils\BoostRandomHelper.h"
#include "Math\MathUtils.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitAttackState.h"

#include "RabbitHitAnimationState.h"
#include "RabbitIdleAnimationState.h"
#include "RabbitDefenseAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Flee.h"
#include "Steering Behaviors\Seek.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDefenseState::CRabbitDefenseState( void )
	: CState			("CRabbitDefenseState")
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pRabbit			( NULL )
	, m_HitBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}

CRabbitDefenseState::CRabbitDefenseState( const std::string &_Name )
	: CState			(_Name)
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pRabbit			( NULL )
	, m_HitBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}


CRabbitDefenseState::~CRabbitDefenseState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDefenseState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}
	
	m_ActionTime.StartAction();

	// Me dice si bloqueo
	m_HitBlocked = 0;

	// Me dice la distancia que recorro cuando pega el player y bloqueo hacia atras
	m_HitDistance = m_pRabbit->GetProperties()->GetImpactDistance() + 2;
	
	// Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
	m_TotalHitBlocked = BoostRandomHelper::GetInt(1, 4);  		
	// print_logger (1, "nº hits totals x blojar"..self.total_hit_blocked )
		
	// me dice el nº de veces que el player me pega mientras bloqueo
	m_HitBlockedCount = 0;
		
	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pRabbit->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pRabbit->GetSteeringEntity()->GetMass();
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(1);
	m_pRabbit->GetSteeringEntity()->SetMass(0.00500f);

	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Defense");
		}
	#endif
}

void CRabbitDefenseState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );

	if ( m_HitBlocked ) 
	{
		float l_Distance = m_pRabbit->GetDistanceToPlayer();
		// Si aun no he hecho el retroceso lo sigo moviendo
		//if ( l_Distance <= m_HitDistance ) 
		//{
		//	//m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
		//}
		// Si llego al destino paro el retroceso
		//else
		//{
		if ( l_Distance > m_HitDistance ) 
		{
			m_pRabbit->GetBehaviors()->SeekOn();
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			//m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_HitBlocked = false;
		} 
		//m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );
		m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
	}	
	else 
	{
		// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( m_pRabbit->IsPlayerAtacable() )
		{
			// Si asoleixo el total de hits bloquejats retorno al anterior estat
			if ( m_HitBlockedCount == m_TotalHitBlocked ) 
			{
				//print_logger (1, "retorno")
				m_pRabbit->SetReceivedHitsXMinut(0);
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
				return;
			}
			
			// Solo hago la acción si estoy dentro de la distancia de impacto
			float l_Distance = m_pRabbit->GetDistanceToPlayer();
			if ( l_Distance <= ( m_pRabbit->GetProperties()->GetImpactDistance() * 2 ) ) 
			{
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetDefenseAnimationState());
			}
			else 
			{
				m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
				m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}		

			if ( m_ActionTime.IsActionFinished() ) 
			{
				// nos volvemos
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());		
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());		
			}
			else 
			{
				// Incrementamos el tiempo que llevamos en este estado
				m_ActionTime.Update(_ElapsedTime);
			}
			//m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		}
		// Si el player NO es atacable lo volvemos a preparar o a perseguir
		else 
		{
			m_pRabbit->SetReceivedHitsXMinut(0);
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetAttackState());		
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());		
		}
	}

}

void CRabbitDefenseState::OnExit( CCharacter* _Character )
{
	m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pRabbit->GetBehaviors()->SeekOff();

	// Restauramos la velocidad original
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pRabbit->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CRabbitDefenseState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// Solo me pongo en hit si realmente he finalizado el estado de ataque
			
		// Me dice que acabo de bloquear un golpe
		m_HitBlocked = true;

		// _CCharacter.behaviors:flee_on()
		m_pRabbit->GetBehaviors()->SeekOn();
		Vect3f l_Front = m_pRabbit->GetSteeringEntity()->GetFront();
		l_Front.Normalize();
		l_Front = l_Front.RotateY(mathUtils::PiTimes(1.f));
			
		//m_pRabbit->GetBehaviors()->GetFlee()->SetTarget(m_pRabbit->GetPlayer()->GetPosition());
		// l_target = Vect3f(l_front.x,l_front.y,l_front.z):normalize(1)
		// l_target = l_target * 2
		// _CCharacter.behaviors.flee.target = l_target
			
		l_Front = m_pRabbit->GetSteeringEntity()->GetPosition() + l_Front * m_HitDistance;
		// _CCharacter.behaviors.flee.target = l_front
		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(l_Front);
					
		// _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )

		// Cuento el nº de hits que lleva mientras bloqueo
		m_HitBlockedCount += 1;
		//print_logger (1, "self.hit_blocked_count : "..self.hit_blocked_count)
		return true;
	}

	return false;
}