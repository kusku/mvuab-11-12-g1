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

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerIdleState::CPlayerIdleState( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
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

	// Ahora debemos actualizar las partículas
	//UpdateParticlesPositions(_pCharacter);

	UpdateImpact(_pCharacter);
	GenerateImpact(_pCharacter);
}

void CPlayerIdleState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	// Ahora debemos actualizar las partículas
	UpdateParticlesPositions(_pCharacter);

	// Gestión de partículas
	UpdateImpact(_pCharacter);
	GenerateImpact(_pCharacter);
	
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
			l_pPlayer->UpdateCamera(_fElapsedTime, true);
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
	/*GetParticleEmitter("BloodSplash")->StopEjectParticles();
	GetParticleEmitter("Impact")->StopEjectParticles();
	GetParticleEmitter("ExpandWave")->StopEjectParticles();
	GetParticleEmitter("Streaks")->StopEjectParticles();*/
}

bool CPlayerIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	if( _Message.Msg == Msg_Attack )
	{
		CRandom	l_Randomize;

		CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pEnemy->GetProperties()->GetStrong() / 2), (float)l_pEnemy->GetProperties()->GetStrong());
		float l_fPainToHit		= l_pEnemy->GetProperties()->GetStrong() * 0.95f;

		/*if( l_fReceivedPain >= l_fPainToHit )
		{*/
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("hit") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animhit") );
		//}

		return true;
	}
	else if( _Message.Msg == Msg_Push )
	{
		/*CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		
		sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
		_pCharacter->MoveTo2(l_Info->Direccion*2.0f, l_Info->ElapsedTime);
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/

		return true;
	}

	return false;
}

void CPlayerIdleState::UpdateParticlesPositions( CCharacter* _pCharacter )
{
}

void CPlayerIdleState::GenerateImpact( CCharacter* _pCharacter )
{
	//GetParticleEmitter("FireSwordBlur")->EjectParticles();
	//GetParticleEmitter("FireSwordSmoke")->EjectParticles();
	/*GetParticleEmitterInstance("DeerBloodSplash", _pCharacter->GetName() + "_DeerBloodSplash")->EjectParticles();
	GetParticleEmitterInstance("DeerBloodDust",	  _pCharacter->GetName() + "_DeerBloodDust")->EjectParticles();
	GetParticleEmitterInstance("DeerBlood",		  _pCharacter->GetName() + "_DeerBlood")->EjectParticles();*/
}

void CPlayerIdleState::UpdateImpact( CCharacter* _pCharacter )
{
	Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	
	SetParticlePosition(_pCharacter, "DeerBloodSplash", _pCharacter->GetName() + "_DeerBloodSplash", "", l_Pos );
	SetParticlePosition(_pCharacter, "DeerBloodDust",	_pCharacter->GetName() + "_DeerBloodDust",	 "", l_Pos);
	SetParticlePosition(_pCharacter, "DeerBlood",		_pCharacter->GetName() + "_DeerBlood",	"", l_Pos);
	/*SetParticlePosition(_pCharacter, "FireSwordBlur", "CHR_CAP L Hand" );
	SetParticlePosition(_pCharacter, "FireSwordSmoke", "CHR_CAP L Hand" );*/

	//Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	//l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	//SetParticlePosition(_pCharacter, "BloodSplash", "", l_Pos );
	//SetParticlePosition(_pCharacter, "Impact", "", l_Pos );
	//SetParticlePosition(_pCharacter, "Streaks", "", l_Pos );

	////l_Pos.z -= 10.f;
	//SetParticlePosition(_pCharacter, "ExpandWave", "", l_Pos );
	
}
