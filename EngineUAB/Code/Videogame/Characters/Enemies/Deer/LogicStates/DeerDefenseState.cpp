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
#include "Characters\StatesDefs.h"

#include "DeerPursuitState.h"
#include "DeerPreparedToAttackState.h"
#include "DeerAttackState.h"
#include "DeerIdleState.h"

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
CDeerDefenseState::CDeerDefenseState( CCharacter* _pCharacter )
	: CState			(_pCharacter, "CDeerDefenseState")
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pDeer			( NULL )
	, m_HitIsBlocked		( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}

CDeerDefenseState::CDeerDefenseState( CCharacter* _pCharacter, const std::string &_Name )
	: CState			(_pCharacter, _Name)
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pDeer			( NULL ), m_HitIsBlocked		( false )
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
void CDeerDefenseState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	
	m_ActionTime.InitAction(0, m_pDeer->GetProperties()->GetTimeInDefense());
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
			std::string l_State = DEER_DEFENSE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerDefenseState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Si se recibe golpeo entonces retrasamos unos metros
	if ( m_HitIsBlocked ) 
	{
		UpdateImpact(_pCharacter);
		float l_Distance = m_pDeer->GetPosition().Distance(m_InitialHitPoint);
		if ( l_Distance >= m_MaxHitDistance ) 
		{
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_HitIsBlocked = false;
			m_pDeer->SetReceivedHitsXMinut(0);
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
			return;
		} 
		else
		{
			m_pDeer->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}	
	// Si no se recibe golpeo
	else 
	{
		// Siempre miro al player mientras bloqueo
		m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );

		// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( m_pDeer->IsPlayerAtacable() )
		{
			// Si asoleixo el total de hits bloquejats retorno al anterior estat
			if ( m_HitBlockedCount == m_TotalHitBlocked ) 
			{
				//print_logger (1, "retorno")
				m_pDeer->SetReceivedHitsXMinut(0);
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());
				m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
				return;
			}
			
			// Solo hago la acción si estoy dentro de la distancia de impacto
			float l_Distance = m_pDeer->GetDistanceToPlayer();
			if ( l_Distance <= ( m_pDeer->GetProperties()->GetImpactDistance() * 3 ) ) 
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
				m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());		
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
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());		
			m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());		
		}
	}

}

void CDeerDefenseState::OnExit( CCharacter* _pCharacter )
{
	m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pDeer->GetBehaviors()->SeekOff();

	// Restauramos la velocidad original
	m_pDeer->GetSteeringEntity()->SetMaxSpeed(m_pDeer->GetProperties()->GetMaxSpeed());
	
	StopImpact(_pCharacter);
}

bool CDeerDefenseState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
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
		m_pDeer->SetPlayerHasBeenReached( false );

		// Guardamos la posición inicial de impacto
		m_InitialHitPoint = m_pDeer->GetPosition();

		// --- Para la gestión del retroceso ---
		m_pDeer->GetProperties()->SetMaxSpeed(m_pDeer->GetProperties()->GetHitRecoilSpeed());

		m_HitDirection = m_pDeer->GetSteeringEntity()->GetFront();
		m_HitDirection.Normalize();
		m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
		
		// Me dice la distancia máxima y posición que recorro cuando pega el player y bloqueo hacia atras
		m_MaxHitDistance = m_pDeer->GetProperties()->GetHitRecoilDistance();
		// ---------------------------------------

		m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_HitDirection);
					
		// Cuento el nº de hits que lleva mientras bloqueo
		m_HitBlockedCount += 1;

		// tratamos el tema de partículas de bloqueo
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
		return true;
	}

	return false;
}

void CDeerDefenseState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("DeerBlockedSparks",	     _pCharacter->GetName() + "_DeerBlockedSparks")->EjectParticles();
	GetParticleEmitterInstance("DeerBlockedExpandWave",  _pCharacter->GetName() + "_DeerBlockedExpandWave")->EjectParticles();
}

void CDeerDefenseState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = m_InitialHitPoint;
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "DeerBlockedSparks",	    _pCharacter->GetName() + "_DeerBlockedSparks",	   "", l_Pos);
	SetParticlePosition(_pCharacter, "DeerBlockedExpandWave",	_pCharacter->GetName() + "_DeerBlockedExpandWave", "", l_Pos);
}

void CDeerDefenseState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("DeerBlockedSparks",	    _pCharacter->GetName() + "_DeerBlockedSparks")->StopEjectParticles();
	GetParticleEmitterInstance("DeerBlockedExpandWave", _pCharacter->GetName() + "_DeerBlockedExpandWave")->StopEjectParticles();
}