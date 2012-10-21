#include "PlayerDefenseState.h"
#include "StatesMachine\Telegram.h"
#include "PhysicController.h"
#include "ActionToInput.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "GameProcess.h"
#include "Characters/Player/Player.h"
#include "SoundManager.h"
#include "Utils\Timer.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Characters\StatesDefs.h"


#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerDefenseState::CPlayerDefenseState( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
{
	m_pInput = CORE->GetActionToInput();
}

CPlayerDefenseState::~CPlayerDefenseState()
{
	m_pInput = NULL;
}

void CPlayerDefenseState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Defense");
	}
#endif

	m_IsAttacked = false;
	m_DoubleHit = false;
}

void CPlayerDefenseState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( !m_pInput->DoAction("DefensePlayer") )
	{
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
	}

	// -----------------------------------------------------
	// Si no golpean simplemente pongo en modo defensa
	// -----------------------------------------------------
	if (!m_IsAttacked )
	{
		_pCharacter->GetController()->Move( v3fZERO, _fElapsedTime);
		return;
	}

	// ---------------------------------------
	// En caso de golpeo mientras defiendo
	// ---------------------------------------
	
	// En caso que recibamos otro hit mientras estemos en este estado ampliamos el recorrido
	if ( m_DoubleHit )
	{
		LOGGER->AddNewLog(ELL_INFORMATION,"CPlayerDefenseState::Execute-> Double Hit done!");	
		CalculateRecoilDirection(_pCharacter);
		
		// Esto permite que ya no reste vida ni se recalcule nada
		m_DoubleHit = false;
	}

	// Gestiono el retroceso del hit
	float l_Distance = _pCharacter->GetPosition().Distance(m_InitialHitPoint);
	if ( l_Distance >= m_MaxHitDistance ) 
	{
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		_pCharacter->MoveTo2( _pCharacter->GetSteeringEntity()->GetVelocity(), _fElapsedTime );
	} 
	else
	{
		_pCharacter->MoveTo2(m_HitDirection, _fElapsedTime );
	}
}

void CPlayerDefenseState::OnExit( CCharacter* _pCharacter )
{
	_pCharacter->SetLocked(false);
}

bool CPlayerDefenseState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Caperucita_combo_hit");

 	if ( _Message.Msg == Msg_Attack || _Message.Msg == Msg_Push ) 
	{
		m_Message = _Message;
		CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
		m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);

		CalculateRecoilDirection(_pCharacter);
		_pCharacter->FaceToForPlayer( m_pEnemy->GetPosition(), CORE->GetTimer()->GetElapsedTime() );

		if (m_IsAttacked)
		{
			m_DoubleHit = true;
			LOGGER->AddNewLog(ELL_INFORMATION,"CPlayerDefenseState::OnMessage-> Posem el doble hit!");	
		}
		else
		{
			m_IsAttacked = true;
		}
		return true;
	}
	return false;
}

void CPlayerDefenseState::CalculateRecoilDirection( CCharacter * _pCharacter ) 
{
	LOGGER->AddNewLog( ELL_WARNING, "CPlayerDefenseState::CalculateRecoilDirection" );

	// Calculamos la dirección y fuerza de retroceso a partir del tipo de mensaje recibido
	CProperties * l_Properties = _pCharacter->GetProperties();
	m_MaxHitSpeed = l_Properties->GetHitRecoilSpeed();
	m_MaxHitDistance = l_Properties->GetHitRecoilDistance();
	m_InitialHitPoint = _pCharacter->GetPosition();
	
	// Cojemos la dirección que se recibe del atacante 
	Vect3f l_EnemyFront = m_pEnemy->GetFront();
	Vect3f l_PlayerFront = _pCharacter->GetFront();
	_pCharacter->SetLocked(true);
	
	if ( m_Message.Msg == Msg_Attack )
	{
		// Si no tiene velocidad el player rotamos hacia el enemigo y reculamos en su dirección
		if ( _pCharacter->GetSteeringEntity()->GetSpeed() == 0 )
		{
			//m_HitDirection = l_EnemyFront.RotateY(mathUtils::PiTimes(1.f));	
			m_HitDirection = l_EnemyFront;
			m_HitDirection *= m_MaxHitSpeed;
		}
		// Si tenemos los dos velocidad el vector resultante indicará la dirección a seguir y recular
		else
		{
			// Dirección segun la suma de vectores
			m_HitDirection = l_EnemyFront + l_PlayerFront;
		}
		LOGGER->AddNewLog( ELL_WARNING, "m_Message.Msg == Msg_Attack" );
	}
	else if ( ( m_Message.Msg == Msg_Push ) )
	{
		m_HitDirection = m_pEnemy->GetSteeringEntity()->GetVelocity();
		m_PushHit = true; 
	}
}
