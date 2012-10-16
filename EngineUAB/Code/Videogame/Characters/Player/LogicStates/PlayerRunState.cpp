#include "PlayerRunState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "Characters\Player\Player.h"
#include "Characters\Player\LogicStates\PlayerHitState.h"
#include "Utils\Random.h"
#include "ActionToInput.h"
#include "GameProcess.h"
#include "EngineProcess.h"
#include "Helpers\MathHelper.h"
#include "PhysicController.h"
#include "SoundManager.h"
#include "Core.h"
#include "Base.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Characters\StatesDefs.h"
#include "Logger\Logger.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerRunState::CPlayerRunState( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
	, m_fRotationVelocity( _pCharacter->GetProperties()->GetMaxRotationSpeed() )
	, m_fMaxVelocityMovement( _pCharacter->GetProperties()->GetMaxSpeed() )
	, m_fCurrentVelocityMovement(0.f)
	, m_fAccelerationStart( _pCharacter->GetProperties()->GetMinAcceleration() )
	, m_fAccelerationEnd( _pCharacter->GetProperties()->GetMaxAcceleration() )
	, m_bStartState(true)
	, m_bEndState(false)
	, m_LastDirection(v3fZERO)
	, m_pPlayer(NULL)
{
	m_pProcess	= static_cast<CGameProcess*>(CORE->GetProcess());
	m_pInput	= CORE->GetActionToInput();
}

CPlayerRunState::~CPlayerRunState()
{
	m_pProcess	= NULL;
	m_pPlayer	= NULL;
}

void CPlayerRunState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Run");
	}
#endif

	if( m_pPlayer == NULL )
		m_pPlayer = static_cast<CPlayer*>( _pCharacter );

	m_LastDirection				= v3fZERO;
	m_fCurrentVelocityMovement	= 0.f;
	m_bStartState				= true;
	m_bEndState					= false;

	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Caperucita_fast_sighs");
}

void CPlayerRunState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	CCharacter *l_pEnemy		= NULL; 

	if( !_pCharacter->GetLocked() )
	{
		//El jugador ataca
		if( m_pInput->DoAction("HardAttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack4") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack4") );
		}
		else if( m_pInput->DoAction("AttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack1") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack1") );
		}

		float l_fYaw = _pCharacter->GetYaw();

		//Mira los controles de movimiento
		bool l_bMovePlayer	= false;
		Vect3f l_Dir		= v3fZERO;

		if( m_pInput->DoAction("MovePlayerUp") )
		{
			if( m_pInput->DoAction("MovePlayerLeft") )
			{
				l_fYaw += FLOAT_PI_VALUE / 4.f;
			}
			else if( m_pInput->DoAction("MovePlayerRight") )
			{
				l_fYaw -= FLOAT_PI_VALUE / 4.f;
			}

			l_bMovePlayer = true;
		}
		else if( m_pInput->DoAction("MovePlayerDown") )
		{
			l_fYaw -= FLOAT_PI_VALUE;

			if( m_pInput->DoAction("MovePlayerLeft") )
			{
				l_fYaw -= FLOAT_PI_VALUE / 4.f;
			}
			else if( m_pInput->DoAction("MovePlayerRight") )
			{
				l_fYaw += FLOAT_PI_VALUE / 4.f;
			}

			l_bMovePlayer = true;
		}
		else if( m_pInput->DoAction("MovePlayerLeft") )
		{
			l_fYaw += FLOAT_PI_VALUE / 2.f;
			l_bMovePlayer = true;
		}
		else if( m_pInput->DoAction("MovePlayerRight") )
		{
			l_fYaw -= FLOAT_PI_VALUE / 2.f;
			l_bMovePlayer = true;
		}


		if( l_bMovePlayer )
		{
			l_Dir = Vect3f( mathUtils::Cos<float>(l_fYaw), 0.f, mathUtils::Sin<float>(l_fYaw) );
			m_LastDirection = l_Dir;

			if( m_bEndState )
			{
				m_bStartState = true;
				m_bEndState = false;
			}
		}
		else
		{
			l_Dir = m_LastDirection;
			m_bEndState = true;
		}


		//La aplica la velocidad al movimiento
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
		
		l_Dir = l_Dir * m_fCurrentVelocityMovement * _fElapsedTime;

		//Mueve el controller físico
		_pCharacter->GetController()->Move( l_Dir, _fElapsedTime );
		_pCharacter->GetSteeringEntity()->SetVelocity(l_Dir);

		if( l_bMovePlayer )
		{
			//Pone de forma correcta los ángulos
			_pCharacter->GetController()->SetYaw( l_fYaw );
			_pCharacter->GetAnimatedModel()->SetYaw( -mathUtils::Rad2Deg(l_fYaw) + 90.f );

			//Mira si se hace un salto
			if( m_pInput->DoAction("PlayerJump") )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("jump") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animjump") );
			}
		}
		else if( !l_bMovePlayer && !m_bEndState )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
		}
	}
}

void CPlayerRunState::OnExit( CCharacter* _pCharacter )
{
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Stop_EFX_Caperucita_fast_sighs");
}

bool CPlayerRunState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	CPlayer * l_pPlayer = dynamic_cast<CPlayer*> (_pCharacter);
	
	//if( _Message.Msg == Msg_Attack )
	//{
	//	CRandom	l_Randomize;

	//	CCharacter *l_pEnemy	= m_pProcess->GetCharactersManager()->GetCharacterById(_Message.Sender);
	//	float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pEnemy->GetProperties()->GetStrong() / 2), (float)l_pEnemy->GetProperties()->GetStrong());
	//	float l_fPainToHit		= l_pEnemy->GetProperties()->GetStrong() * 0.95f;

	//	if( l_fReceivedPain >= l_fPainToHit )
	//	{
	//		return l_pPlayer->CallHitState(_pCharacter, _Message);
	//	}

	//	return true;
	//}
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
	return l_pPlayer->CallHitState(_pCharacter, _Message);
}