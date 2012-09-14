#include "PlayerIdleState.h"
#include "StatesMachine\Telegram.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "ActionToInput.h"
#include "Helpers\MathHelper.h"
#include "Utils\Random.h"
#include "GameProcess.h"
#include "EngineProcess.h"

#include "Characters\CharacterManager.h"
#include "Characters\Player\Player.h"
#include "Characters\StatesDefs.h"

#include "Steering Behaviors\SteeringEntity.h"

#include "Cameras\ThPSCharacterCamera.h"
#include "PhysicController.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerIdleState::CPlayerIdleState( const std::string &_Name )
	: CState(_Name)
{

}

CPlayerIdleState::~CPlayerIdleState()
{

}

void CPlayerIdleState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Idle");
	}
#endif
}

void CPlayerIdleState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( !_pCharacter->GetLocked() )
	{
		CPlayer *l_pPlayer				= static_cast<CPlayer*>(_pCharacter);
		CCharacter *l_pEnemyDetected	= l_pPlayer->DetectEnemy();

		if( l_pPlayer->IsTargetFixed() )
		{
			if( l_pEnemyDetected != NULL )
			{
				if( !l_pEnemyDetected->IsAlive() )
				{
					static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SetTargetEnemy(NULL);
					l_pPlayer->SetTargetFixed(false);
				}
			}
			else
			{
				static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SetTargetEnemy(NULL);
				l_pPlayer->SetTargetFixed(false);
			}
		}

		if( CORE->GetActionToInput()->DoAction("PlayerTarget") )
		{
			//Se asigna un target
			if( !l_pPlayer->IsTargetFixed() )
			{
				if( l_pEnemyDetected != NULL )
				{
					static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SetTargetEnemy( l_pEnemyDetected );
					l_pPlayer->SetTargetFixed(true);
				}
			}
			else
			{
				static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SetTargetEnemy(NULL);
				l_pPlayer->SetTargetFixed(false);
			}
		}

		if( CORE->GetActionToInput()->DoAction("AttackPlayer") )
		{
			//El jugador ataca
			if( l_pPlayer->IsTargetFixed() )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("targetattack1") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack1") );
			}
			else
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack1") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack1") );
			}
		}
		else if(	CORE->GetActionToInput()->DoAction("MovePlayerUp") || 
					CORE->GetActionToInput()->DoAction("MovePlayerDown") || 
					CORE->GetActionToInput()->DoAction("MovePlayerLeft") ||
					CORE->GetActionToInput()->DoAction("MovePlayerRight") )
		{
			if( l_pPlayer->IsTargetFixed() )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("targetrun") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animrun") );
			}
			else
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("run") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animrun") );
			}
		}

		//Actualización del yaw a partir del movimiento del mouse
		if( !l_pPlayer->IsTargetFixed() )
		{
			float l_fDelta = CORE->GetActionToInput()->DoActionMouse("YawPlayer");

			_pCharacter->SetYaw( Helper::AngleFilter(_pCharacter->GetYaw() - l_fDelta) );
		}
		else
		{
			Vect3f l_PlayerDir	= static_cast<CGameProcess*>(CORE->GetProcess())->GetPlayerCamera()->GetDirection();
			Vect3f l_EnemyDir	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetTargetEnemy()->GetPosition() - _pCharacter->GetPosition();

			l_PlayerDir.y		= 0.f;
			l_EnemyDir.y		= 0.f;

			l_PlayerDir.Normalize(1.f);
			l_EnemyDir.Normalize(1.f);

			float l_fDelta		= l_EnemyDir.Dot( l_PlayerDir );
			l_fDelta			= mathUtils::ACos<float>( l_fDelta );

			if( mathUtils::Rad2Deg( l_fDelta ) > 0.1f )
			{
				_pCharacter->SetYaw( Helper::AngleFilter(_pCharacter->GetYaw() - l_fDelta) );
			}
		}
	}

	//Actualiza el personaje
	_pCharacter->GetController()->Move( v3fZERO, _fElapsedTime );
}

void CPlayerIdleState::OnExit( CCharacter* _pCharacter )
{
}

bool CPlayerIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	if( _Message.Msg == Msg_Attack )
	{
		CRandom	l_Randomize;

		CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pEnemy->GetProperties()->GetStrong() / 2), (float)l_pEnemy->GetProperties()->GetStrong());
		float l_fPainToHit		= l_pEnemy->GetProperties()->GetStrong() * 0.95f;

		if( l_fReceivedPain >= l_fPainToHit )
		{
			static_cast<CPlayer*>(_pCharacter)->HitToPlayer();
		}

		return true;
	}
	else if( _Message.Msg == Msg_Push )
	{
		CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		
		sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
		_pCharacter->MoveTo2(l_Info->Direccion * 1.2f, l_Info->ElapsedTime);
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerIdleState::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}