#include "WolfDefenseState.h"
#include "Utils\BoostRandomHelper.h"
#include "Math\MathUtils.h"
#include "SoundManager.h"

#include "Characters\Enemies\Wolf\Wolf.h"
#include "Characters\StatesDefs.h"

#include "WolfPursuitState.h"
#include "WolfPreparedToAttackState.h"
#include "WolfAttackState.h"
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfDefenseAnimationState.h"

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
CWolfDefenseState::CWolfDefenseState( CCharacter* _pCharacter )
	: CState			(_pCharacter, "CWolfDefenseState")
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pWolf			( NULL )
	, m_HitIsBlocked	( false )
	, m_TotalHitBlocked	( 0 )
	, m_HitBlockedCount	( 0 )
{
}

CWolfDefenseState::CWolfDefenseState( CCharacter* _pCharacter, const std::string &_Name )
	: CState			(_pCharacter, _Name)
	, m_ActionTime		( CActionStateCallback( 0.f, 6.f ) )
	, m_pWolf			( NULL )
	, m_HitIsBlocked	( false )
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
void CWolfDefenseState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_ActionTime.InitAction(0, m_pWolf->GetProperties()->GetTimeInDefense());
	m_ActionTime.StartAction();

	// Me dice si bloqueo
	m_HitIsBlocked = false;

	// Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
	m_TotalHitBlocked = BoostRandomHelper::GetInt(1, 4);  		
		
	// me dice el nº de veces que el player me pega mientras bloqueo
	m_HitBlockedCount = 0;

	CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Pain");

	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = WOLF_DEFENSE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfDefenseState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	// Si se recibe golpeo entonces retrasamos unos metros
	if ( m_HitIsBlocked ) 
	{
		UpdateImpact(_pCharacter);
		float l_Distance = m_pWolf->GetPosition().Distance(m_InitialHitPoint);
		if ( l_Distance >= m_MaxHitDistance ) 
		{
			m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
			m_HitIsBlocked = false;
			m_pWolf->SetReceivedHitsXMinut(0);
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
			return;
		} 
		else
		{
			m_pWolf->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}	
	// Si no se recibe golpeo
	else 
	{
		// Siempre miro al player mientras bloqueo
		m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime );

		// Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
		if ( m_pWolf->IsPlayerAtacable() )
		{
			// Si asoleixo el total de hits bloquejats retorno al anterior estat
			if ( m_HitBlockedCount == m_TotalHitBlocked ) 
			{
				//print_logger (1, "retorno")
				m_pWolf->SetReceivedHitsXMinut(0);
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
				return;
			}
			
			// Solo hago la acción si estoy dentro de la distancia de impacto
			float l_Distance = m_pWolf->GetDistanceToPlayer();
			if ( l_Distance <= ( m_pWolf->GetProperties()->GetImpactDistance() * 3 ) ) 
			{
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetDefenseAnimationState());
				return;
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
				m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());		
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());		
			}
			else 
			{
				// Incrementamos el tiempo que llevamos en este estado
				m_ActionTime.Update(_ElapsedTime);
			}
			//m_pWolf->MoveTo2( m_pWolf->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		}
		// Si el player NO es atacable lo volvemos a preparar o a perseguir
		else 
		{
			m_pWolf->SetReceivedHitsXMinut(0);
			m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetIdleState());		
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());		
		}
	}
}

void CWolfDefenseState::OnExit( CCharacter* _pCharacter )
{
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->GetBehaviors()->SeekOff();

	// Restauramos la velocidad original
	m_pWolf->GetSteeringEntity()->SetMaxSpeed(m_pWolf->GetProperties()->GetMaxSpeed());
	StopImpact(_pCharacter);
}

bool CWolfDefenseState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		// Solo me pongo en hit si realmente he finalizado el estado de ataque
			
		// Si aun estoy en hit no vuelvo a animar
		if ( m_HitIsBlocked )
		{
			return false;
		}

		// Me dice que acabo de bloquear un golpe
		m_HitIsBlocked = true;

		// Indicamos que el player no se pilló
		m_pWolf->SetPlayerHasBeenReached( false );

		// Guardamos la posición inicial de impacto
		m_InitialHitPoint = m_pWolf->GetPosition();

		// --- Para la gestión del retroceso ---
		CProperties * l_Properties = m_pWolf->GetProperties();
		float l_MaxHitSpeed = l_Properties->GetHitRecoilSpeed();
		m_pWolf->GetSteeringEntity()->SetMaxSpeed(l_MaxHitSpeed);
		// Me dice la distancia máxima y posición que recorro cuando pega el player y bloqueo hacia atras
		m_MaxHitDistance = l_Properties->GetHitRecoilDistance();
		
		m_HitDirection = m_pWolf->GetSteeringEntity()->GetFront();
		m_HitDirection.Normalize();
		m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));
		m_HitDirection = m_HitDirection * l_MaxHitSpeed;

		// ---------------------------------------

		m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		m_pWolf->GetBehaviors()->GetSeek()->SetTarget(m_HitDirection);
					
		// Cuento el nº de hits que lleva mientras bloqueo
		m_HitBlockedCount += 1;

		// tratamos el tema de partículas de bloqueo
		UpdateImpact(_pCharacter);
		GenerateImpact(_pCharacter);
		return true;
	}

	return false;
}

void CWolfDefenseState::GenerateImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfBlockedSparks",	     _pCharacter->GetName() + "_WolfBlockedSparks")->EjectParticles();
	GetParticleEmitterInstance("WolfBlockedExpandWave",  _pCharacter->GetName() + "_WolfBlockedExpandWave")->EjectParticles();
}

void CWolfDefenseState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = m_InitialHitPoint;
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "WolfBlockedSparks",	    _pCharacter->GetName() + "_WolfBlockedSparks",	   "", l_Pos);

	l_Pos = _pCharacter->GetPosition();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	SetParticlePosition(_pCharacter, "WolfBlockedExpandWave",	_pCharacter->GetName() + "_WolfBlockedExpandWave", "", l_Pos);
}

void CWolfDefenseState::StopImpact( CCharacter* _pCharacter )
{
	GetParticleEmitterInstance("WolfBlockedSparks",	    _pCharacter->GetName() + "_WolfBlockedSparks")->StopEjectParticles();
	GetParticleEmitterInstance("WolfBlockedExpandWave", _pCharacter->GetName() + "_WolfBlockedExpandWave")->StopEjectParticles();
}