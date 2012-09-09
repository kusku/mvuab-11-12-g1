#include "WolfDefenseState.h"
#include "Utils\BoostRandomHelper.h"
#include "Math\MathUtils.h"

#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfAttackState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"

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
CWolfDefenseState::CWolfDefenseState( void )
	: CState			("CWolfDefenseState")
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pWolf			( NULL )
	, m_HitBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}

CWolfDefenseState::CWolfDefenseState( const std::string &_Name )
	: CState			(_Name)
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pWolf			( NULL )
	, m_HitBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}


CWolfDefenseState::~CWolfDefenseState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfDefenseState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	m_ActionTime.StartAction();

	// Me dice si bloqueo
	m_HitBlocked = 0;

	// Me dice la distancia que recorro cuando paga y bloqueo hacia atras
	m_HitDistance = m_pWolf->GetProperties()->GetImpactDistance() + 2;
	
	// Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
	m_TotalHitBlocked = BoostRandomHelper::GetInt(1, 4);  		
	// print_logger (1, "nº hits totals x blojar"..self.total_hit_blocked )
		
	// me dice el nº de veces que el player me pega mientras bloqueo
	m_HitBlockedCount = 0;
		
	// Metemos más velocidad al ataque i menos massa para acelerar más 
	m_OldMaxSpeed = m_pWolf->GetSteeringEntity()->GetMaxSpeed();
	m_OldMass = m_pWolf->GetSteeringEntity()->GetMass();
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(1);
	m_pWolf->GetSteeringEntity()->SetMass(0.00500f);

	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			CORE->GetDebugGUIManager()->GetDebugRender()->SetEnemyStateName("Defense");
		}
	#endif
}

void CWolfDefenseState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	if ( m_HitBlocked ) 
	{
		float l_Distance = m_pWolf->GetDistanceToPlayer();
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
			// _CCharacter.behaviors:flee_off()
			m_pWolf->GetBehaviors()->SeekOn();
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			//m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_HitBlocked = false;
		} 
		m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
	}	
	else 
	{
		// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( m_pWolf->IsPlayerAtacable() )
		{
			// Si asoleixo el total de hits bloquejats retorno al anterior estat
			if ( m_HitBlockedCount == m_TotalHitBlocked ) 
			{
				//print_logger (1, "retorno")
				m_pWolf->SetReceivedHitsXMinut(0);
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetAttackState());
				return;
			}
			
			// Solo hago la acción si estoy dentro de la distancia de impacto
			float l_Distance = m_pWolf->GetDistanceToPlayer();
			if ( l_Distance <= ( m_pWolf->GetProperties()->GetImpactDistance() * 2 ) ) 
			{
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			}
			else 
			{
				m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
				m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			}		

			if ( m_ActionTime.IsActionFinished() ) 
			{
				// nos volvemos
				m_pWolf->GetLogicFSM()->RevertToPreviousState();		
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			}
			else 
			{
				// Incrementamos el tiempo que llevamos en este estado
				m_ActionTime.Update(_ElapsedTime);
			}
		}
		// Si el player NO es atacable lo volvemos a preparar o a perseguir
		else 
		{
			m_pWolf->SetReceivedHitsXMinut(0);
			m_pWolf->GetLogicFSM()->RevertToPreviousState();		
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
		}
	}

}

void CWolfDefenseState::OnExit( CCharacter* _Character )
{
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->GetBehaviors()->SeekOff();

	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_OldMaxSpeed);
	m_pWolf->GetSteeringEntity()->SetMass(m_OldMass);
}

bool CWolfDefenseState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// Solo me pongo en hit si realmente he finalizado el estado de ataque
			
		// Me dice que acabo de bloquear un golpe
		m_HitBlocked = true;

		// _CCharacter.behaviors:flee_on()
		m_pWolf->GetBehaviors()->SeekOn();
		Vect3f l_Front = m_pWolf->GetSteeringEntity()->GetFront();
		l_Front.Normalize();
		l_Front = l_Front.RotateY(mathUtils::PiTimes(1.f));
			
		//m_pWolf->GetBehaviors()->GetFlee()->SetTarget(m_pWolf->GetPlayer()->GetPosition());
		// l_target = Vect3f(l_front.x,l_front.y,l_front.z):normalize(1)
		// l_target = l_target * 2
		// _CCharacter.behaviors.flee.target = l_target
			
		l_Front = m_pWolf->GetSteeringEntity()->GetPosition() + l_Front * m_HitDistance;
		// _CCharacter.behaviors.flee.target = l_front
		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->GetBehaviors()->GetSeek()->SetTarget(l_Front);
					
		// _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )

		// Cuento el nº de hits que lleva mientras bloqueo
		m_HitBlockedCount += 1;
		//print_logger (1, "self.hit_blocked_count : "..self.hit_blocked_count)
		return true;
	}

	return false;
}