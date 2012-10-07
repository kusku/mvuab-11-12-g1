#include "RabbitDefenseState.h"
#include "Utils\BoostRandomHelper.h"
#include "Math\MathUtils.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "Characters\StatesDefs.h"

#include "RabbitPursuitState.h"
#include "RabbitPreparedToAttackState.h"
#include "RabbitAttackState.h"
#include "RabbitIdleState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitDefenseAnimationState.h"

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
CRabbitDefenseState::CRabbitDefenseState( CCharacter* _pCharacter )
	: CState			(_pCharacter, "CRabbitDefenseState")
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pRabbit			( NULL )
	, m_HitIsBlocked	( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}

CRabbitDefenseState::CRabbitDefenseState( CCharacter* _pCharacter, const std::string &_Name )
	: CState			(_pCharacter,_Name)
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pRabbit			( NULL )
	, m_HitIsBlocked	( false )
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
void CRabbitDefenseState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	m_ActionTime.InitAction(0, m_pRabbit->GetProperties()->GetTimeInDefense());
	m_ActionTime.StartAction();

	// Me dice si bloqueo
	m_HitIsBlocked = false;

	// Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
	m_TotalHitBlocked = BoostRandomHelper::GetInt(1, 4);  		
	// print_logger (1, "nº hits totals x blojar"..self.total_hit_blocked )
		
	// me dice el nº de veces que el player me pega mientras bloqueo
	m_HitBlockedCount = 0;
		
	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = RABBIT_DEFENSE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif
}

void CRabbitDefenseState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	// Si se recibe golpeo entonces retrasamos unos metros
	if ( m_HitIsBlocked ) 
	{
		UpdateImpact(_pCharacter);
		float l_Distance = m_pRabbit->GetPosition().Distance(m_InitialHitPoint);
		if ( l_Distance >= m_MaxHitDistance ) 
		{
			m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pRabbit->MoveTo2( m_pRabbit->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_HitIsBlocked = false;
			m_pRabbit->SetReceivedHitsXMinut(0);
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
			return;
		} 
		else
		{
			m_pRabbit->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}	
	// Si no se recibe golpeo
	else 
	{
		// Siempre miro al player mientras bloqueo
		m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime );

		// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( m_pRabbit->IsPlayerAtacable() )
		{
			// Si asoleixo el total de hits bloquejats retorno al anterior estat
			if ( m_HitBlockedCount == m_TotalHitBlocked ) 
			{
				//print_logger (1, "retorno")
				m_pRabbit->SetReceivedHitsXMinut(0);
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());
				m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
				return;
			}
			
			// Solo hago la acción si estoy dentro de la distancia de impacto
			float l_Distance = m_pRabbit->GetDistanceToPlayer();
			if ( l_Distance <= ( m_pRabbit->GetProperties()->GetImpactDistance() * 3 ) ) 
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
				m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());		
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
			m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetIdleState());		
			m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());		
		}
	}
}

void CRabbitDefenseState::OnExit( CCharacter* _pCharacter )
{
	m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pRabbit->GetBehaviors()->SeekOff();

	// Restauramos la velocidad original
	m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_pRabbit->GetProperties()->GetMaxSpeed());
	
	StopImpact(_pCharacter);
}

bool CRabbitDefenseState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	// Solo me pongo en hit si realmente he finalizado el estado de ataque
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// Si aun estoy en hit no vuelvo a animar
		if ( m_HitIsBlocked )
		{
			return false;
		}

		// Me dice que acabo de bloquear un golpe
		m_HitIsBlocked = true;

		// Indicamos que el player no se pilló
		m_pRabbit->SetPlayerHasBeenReached( false );

		// Guardamos la posición inicial de impacto
		m_InitialHitPoint = m_pRabbit->GetPosition();

		// --- Para la gestión del retroceso ---
		m_pRabbit->GetSteeringEntity()->SetMaxSpeed(m_pRabbit->GetProperties()->GetHitRecoilSpeed());

		m_HitDirection = m_pRabbit->GetSteeringEntity()->GetFront();
		m_HitDirection.Normalize();
		m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
		
		// Me dice la distancia máxima y posición que recorro cuando pega el player y bloqueo hacia atras
		m_MaxHitDistance = m_pRabbit->GetProperties()->GetHitRecoilDistance();
		// ---------------------------------------

		m_pRabbit->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pRabbit->GetBehaviors()->GetSeek()->SetTarget(m_HitDirection);
					
		// Cuento el nº de hits que lleva mientras bloqueo
		m_HitBlockedCount += 1;

		// tratamos el tema de partículas de bloqueo
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
		return true;
	}

	return false;
}

void CRabbitDefenseState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBlockedSparks",	   _pCharacter->GetName() + "_RabbitBlockedSparks")->EjectParticles();
	GetParticleEmitterInstance("RabbitBlockedExpandWave",  _pCharacter->GetName() + "_RabbitBlockedExpandWave")->EjectParticles();
}

void CRabbitDefenseState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = m_InitialHitPoint;
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "RabbitBlockedSparks",	    _pCharacter->GetName() + "_RabbitBlockedSparks",	 "", l_Pos);

	l_Pos = _pCharacter->GetPosition();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "RabbitBlockedExpandWave",	_pCharacter->GetName() + "_RabbitBlockedExpandWave", "", l_Pos);
}

void CRabbitDefenseState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("RabbitBlockedSparks",	  _pCharacter->GetName() + "_RabbitBlockedSparks")->StopEjectParticles();
	GetParticleEmitterInstance("RabbitBlockedExpandWave", _pCharacter->GetName() + "_RabbitBlockedExpandWave")->StopEjectParticles();
}