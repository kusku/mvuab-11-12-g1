#include "PlayerTargetAttack2State.h"
#include "StatesMachine\Telegram.h"
#include "ActionToInput.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "PhysicController.h"
#include "StatesMachine\MessageDispatcher.h"
#include "SoundManager.h"
#include "GameProcess.h"
#include "EngineProcess.h"
#include "Utils\Random.h"
#include "Characters\Player\Player.h"
#include "Core.h"
#include "Base.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Characters\StatesDefs.h"
#include "Logger\Logger.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerTargetAttack2State::CPlayerTargetAttack2State( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
	, m_bFirstUpdate(true)
	, m_fMaxVelocityMovement(10.f)
	, m_fCurrentVelocityMovement(10.f)
	, m_fAccelerationMovement(-35.f)
{
	m_pCallback = static_cast<CGameProcess*>(CORE->GetProcess())->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), "attack2");
}

CPlayerTargetAttack2State::~CPlayerTargetAttack2State()
{
}

void CPlayerTargetAttack2State::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Target Attack 2");
	}
#endif

	m_pCallback->StartAnimation();

	m_fCurrentVelocityMovement = m_fMaxVelocityMovement;
	m_bFirstUpdate = true;
}

void CPlayerTargetAttack2State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( m_bFirstUpdate )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_Sword");
		m_bFirstUpdate = false;
	}

	if( m_pCallback->IsAnimationFinished() )
	{
		if( CORE->GetActionToInput()->DoAction("AttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("targetattack3") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack3") );
		}
		else
		{
			if( static_cast<CGameProcess*>(CORE->GetProcess())->GetTimeBetweenClicks() < 0.2f )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("targetattack3") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack3") );
			}
			else
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
			}
		}
	}

	//Movimiento del player hacia adelante
	Vect3f l_Dir = v3fZERO;
	if( !_pCharacter->GetLocked() )
	{
		l_Dir = Vect3f( mathUtils::Cos<float>( _pCharacter->GetYaw() ), 0.f, mathUtils::Sin<float>( _pCharacter->GetYaw() ));
	}

	//Aplica la velocidad al movimiento
	l_Dir = l_Dir * m_fCurrentVelocityMovement * _fElapsedTime;

	//Mueve el controller físico
	CPhysicController *l_pController = _pCharacter->GetController();
	l_pController->Move( l_Dir, _fElapsedTime );

	l_pController->SetYaw( _pCharacter->GetYaw() );

	//Actualiza la velocidad de movimiento
	m_fCurrentVelocityMovement = m_fCurrentVelocityMovement + m_fAccelerationMovement * _fElapsedTime;
	if( m_fCurrentVelocityMovement < 0.f )
	{
		m_fCurrentVelocityMovement = 0.f;
	}
}

void CPlayerTargetAttack2State::OnExit( CCharacter* _pCharacter )
{
	Vect3f l_Front			= _pCharacter->GetAnimatedModel()->GetFront();
	CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SearchTargetEnemy(3.0f, FLOAT_PI_VALUE / 6.f, l_Front);

	if( l_pEnemy != NULL )
	{
		CORE->GetMessageDispatcher()->DispatchStateMessage(SEND_MSG_IMMEDIATELY, _pCharacter->GetID(), l_pEnemy->GetID(), Msg_Attack, NO_ADDITIONAL_INFO);
	}
}

bool CPlayerTargetAttack2State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
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
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerTargetAttack2State::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}