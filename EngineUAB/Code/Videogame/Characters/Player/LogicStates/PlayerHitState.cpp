#include "PlayerHitState.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Player\Player.h"
#include "Characters\CharacterManager.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "ActionToInput.h"
#include "Callbacks/Animation/AnimationCallback.h"
#include "Callbacks/Animation/AnimationCallbackManager.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "PhysicController.h"
#include "EngineProcess.h"
#include "GameProcess.h"
#include "SoundManager.h"
#include "Utils\Timer.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerHitState::CPlayerHitState( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
{
	m_pCallback = static_cast<CGameProcess*>(CORE->GetProcess())->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), _Name);
}

CPlayerHitState::~CPlayerHitState()
{
	m_pCallback = NULL;
}

void CPlayerHitState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Hit");
	}
#endif

	CPlayer *l_pPlayer = static_cast<CPlayer*>(_pCharacter);
	l_pPlayer->HitToPlayer(m_pEnemy->GetProperties()->GetStrong());
	l_pPlayer->ResetTimeDamage();

	m_pCallback->Init();
	m_pCallback->StartAnimation();

	CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Play_EFX_Caperucita_Pain");

	// --- Para la gestión del retroceso ---
	CalculateRecoilDirection(l_pPlayer);

	// ---------------------------------------

	// De momento si entra es el primer golpeo.
	m_DoubleHit = false;
	m_PushHit = false;

	// Gestión de partículas. Metemos sangre!!
	//UpdateImpact(_pCharacter);
	//GenerateImpact(_pCharacter);
}

void CPlayerHitState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	// Actualizamos la posición
	//UpdateImpact(_pCharacter);

	_pCharacter->FaceToForPlayer( m_pEnemy->GetPosition(), _fElapsedTime );

	if( m_pCallback->IsAnimationFinished() )
	{	
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
		return;
	}

	// En caso que recibamos otro hit mientras estemos en este estado ampliamos el recorrido
	if ( m_DoubleHit )
	{
		LOGGER->AddNewLog(ELL_INFORMATION,"CPlayerHitState::Execute-> Double Hit done!");	
		CPlayer *l_pPlayer = static_cast<CPlayer*>(_pCharacter);
		CalculateRecoilDirection(l_pPlayer);
		l_pPlayer ->HitToPlayer(m_pEnemy->GetProperties()->GetStrong());

		// Esto permite que ya no reste vida ni se recalcule nada
		m_DoubleHit = false;
	}
	
	/*if ( m_PushHit )
	{
		CPlayer *l_pPlayer = static_cast<CPlayer*>(_pCharacter);
		CalculateRecoilDirection(l_pPlayer);
	}*/

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

void CPlayerHitState::OnExit( CCharacter* _pCharacter )
{
	static_cast<CPlayer*>(_pCharacter)->ResetTimeDamage();
	_pCharacter->SetLocked(false);
}

bool CPlayerHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	LOGGER->AddNewLog(ELL_INFORMATION,"CPlayerHitState::OnMessage-> hit rebut en el mateix hit!");	
	if ( _Message.Msg == Msg_Attack ) 
	{
		m_DoubleHit = true;
		LOGGER->AddNewLog(ELL_INFORMATION,"CPlayerHitState::OnMessage-> Posem el doble hit!");	
		return true;
	}
	return false;
}

void CPlayerHitState::UpdateParameters( STelegram& _Message )
{
	m_Message = _Message;
	
	CGameProcess *l_pProcess = static_cast<CGameProcess*>(CORE->GetProcess());
	m_pEnemy				 = l_pProcess->GetCharactersManager()->GetCharacterById(m_Message.Sender);
}

void CPlayerHitState::CalculateRecoilDirection( CCharacter * _pCharacter ) 
{
	LOGGER->AddNewLog( ELL_WARNING, "CPlayerHitState::CalculateRecoilDirection" );

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
	else if ( m_Message.Msg == Msg_Push )
	{
		m_HitDirection = m_pEnemy->GetSteeringEntity()->GetVelocity();
		m_HitDirection.y = 0.1f;

		/*if( m_HitDirection.SquaredLength() > 0.00000001f )
		{
			m_HitDirection.Normalize();
		}
		else 
		{
			m_HitDirection = m_pEnemy->GetSteeringEntity()->GetHeading();
		}*/
		//m_HitDirection *= m_pEnemy->GetProperties()->GetMaxSpeed();
		_pCharacter->MoveTo2(m_HitDirection*1.2f, CORE->GetTimer()->GetElapsedTime());
		m_PushHit = true; 
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		LOGGER->AddNewLog( ELL_WARNING, "m_Message.Msg == Msg_Push" );
	}
}


//else if( _Message.Msg == Msg_Push )
	//{
	//	/*CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
	//	
	//	sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
	//	_pCharacter->MoveTo2(l_Info->Direccion * 1.2f, l_Info->ElapsedTime);
	//	_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	//	LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerAttack2State::OnMessage -> PUSHED!!");*/
	//	return l_pPlayer->CallHitState(_pCharacter, _Message);
	//}

	//return false;