#include "PlayerTargetRunState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "GameProcess.h"
#include "EngineProcess.h"
#include "Utils\Random.h"
#include "Characters\Player\Player.h"
#include "Cameras\ThPSCharacterCamera.h"
#include "PhysicController.h"
#include "ActionToInput.h"
#include "Helpers/MathHelper.h"
#include "Core.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerTargetRunState::CPlayerTargetRunState( const std::string &_Name )
	: CState(_Name)
	, m_pTargetEnemy(NULL)
	, m_fMaxVelocityMovement(10.f)
	, m_fCurrentVelocityMovement(0.f)
	, m_fAccelerationStart(30.f)
	, m_fAccelerationEnd(65.f)
	, m_bStartState(true)
	, m_bEndState(false)
{

}

CPlayerTargetRunState::~CPlayerTargetRunState()
{

}

void CPlayerTargetRunState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Target Run");
	}
#endif

	CGameProcess *l_pProcess	= static_cast<CGameProcess*>(CORE->GetProcess());
	m_pTargetEnemy				= l_pProcess->GetCharactersManager()->GetPreviewTargetEnemy();

	if( m_pTargetEnemy == NULL )
	{
		m_pTargetEnemy = static_cast<CPlayer*>(_pCharacter)->DetectEnemy();
	}

	l_pProcess->GetCharactersManager()->SetTargetEnemy( m_pTargetEnemy );

	m_fCurrentVelocityMovement	= 0.f;
	m_bStartState				= true;
	m_bEndState					= false;
}

void CPlayerTargetRunState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	CGameProcess *l_pProcess	= static_cast<CGameProcess*>(CORE->GetProcess());
	CPlayer *l_pPlayer			= static_cast<CPlayer*>(_pCharacter);
	CActionToInput *l_pInput	= CORE->GetActionToInput();

	if( !_pCharacter->GetLocked() )
	{
		if( m_pTargetEnemy != NULL )
		{
			if( !m_pTargetEnemy->IsAlive() )
			{
				l_pProcess->GetCharactersManager()->SetTargetEnemy( NULL );
				l_pPlayer->SetTargetFixed( false );
			}
		}

		//Mira si se desbloquea el target
		if( l_pInput->DoAction("PlayerTarget") )
		{
			l_pPlayer->SetTargetFixed( false );
		}

		//El jugador ataca
		if( l_pInput->DoAction("AttackPlayer") )
		{
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

		//Calcula el ángulo entre donde miras y el enemigo
		Vect3f l_PlayerDir	= l_pProcess->GetPlayerCamera()->GetDirection();
		Vect3f l_EnemyDir	= l_pProcess->GetCharactersManager()->GetTargetEnemy()->GetPosition() - _pCharacter->GetPosition();

		l_PlayerDir.y	= 0.0f;
		l_EnemyDir.y	= 0.0f;

		l_PlayerDir.Normalize(1.f);
		l_EnemyDir.Normalize(1.f);

		float l_fAngle	= l_EnemyDir.Dot( l_PlayerDir );
		l_fAngle		= mathUtils::ACos<float>( l_fAngle );

		if( mathUtils::Rad2Deg( l_fAngle ) > 0.1 )
		{
			_pCharacter->SetYaw( _pCharacter->GetYaw() - l_fAngle );
		}

		Vect3f l_Dir			= v3fZERO;
		bool l_bMovePlayer		= false;
		float l_fYawCharacter	= _pCharacter->GetYaw();

		//Se mueve el personaje con el target asignado
		if( l_pInput->DoAction("MovePlayerUp") )
		{
			l_Dir			= l_EnemyDir;
			l_bMovePlayer	= true;
		}
		else if( l_pInput->DoAction("MovePlayerDown") )
		{
			l_Dir			= -l_EnemyDir;
			l_bMovePlayer	= true;
		}
		else if( l_pInput->DoAction("MovePlayerLeft") )
		{
			float l_fYaw	= l_fYawCharacter + FLOAT_PI_VALUE / 2.f;
			l_Dir			= Vect3f( mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw) );
			l_bMovePlayer	= true;
		}
		else if( l_pInput->DoAction("MovePlayerRight") )
		{
			_pCharacter->SetYaw( l_fYawCharacter + 2.f*l_fAngle);

			float l_fYaw	= _pCharacter->GetYaw() - FLOAT_PI_VALUE / 2.f;
			l_Dir			= Vect3f( mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw) );
			l_bMovePlayer	= true;
		}

		_pCharacter->SetYaw( Helper::AngleFilter( _pCharacter->GetYaw() ) );

		if( l_bMovePlayer )
		{
			m_LastDirection = l_Dir;

			if( m_bEndState )
			{
				m_bStartState	= true;
				m_bEndState		= false;
			}
		}
		else
		{
			l_Dir = m_LastDirection;
			m_bEndState = true;
		}

		if( m_bStartState )
		{
			m_fCurrentVelocityMovement = m_fCurrentVelocityMovement + m_fAccelerationStart * _fElapsedTime;
			if( m_fCurrentVelocityMovement >= m_fMaxVelocityMovement )
			{
				m_fCurrentVelocityMovement = m_fMaxVelocityMovement;
				m_bStartState = false;
			}
		}
		else if( m_bEndState )
		{
			m_fCurrentVelocityMovement = m_fCurrentVelocityMovement - m_fAccelerationEnd * _fElapsedTime;
			if( m_fCurrentVelocityMovement <= 0.f )
			{
				m_fCurrentVelocityMovement = 0.f;
				m_bEndState = false;
			}
		}


		//Aplica la velocidad al movimiento
		l_Dir = l_Dir * m_fCurrentVelocityMovement * _fElapsedTime;

		//Mueve el controller físico
		_pCharacter->GetController()->Move( l_Dir, _fElapsedTime );

		//Pone la rotación del personaje
		_pCharacter->GetController()->SetYaw( l_fYawCharacter );
		_pCharacter->GetAnimatedModel()->SetYaw( -mathUtils::Rad2Deg( l_fYawCharacter) + 90.f );

		//Cambia el estado
		if( !l_bMovePlayer )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
		}
		else
		{
			//Mira si hace un salto
			if( l_pInput->DoAction("PlayerJump") )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("jump") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animjump") );
			}
			else if( !l_pPlayer->IsTargetFixed() )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("run") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animrun") );

				l_pProcess->GetCharactersManager()->SetTargetEnemy( NULL );
			}
		}
	}
}

void CPlayerTargetRunState::OnExit( CCharacter* _pCharacter )
{
}

bool CPlayerTargetRunState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
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
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerTargetRunState::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}