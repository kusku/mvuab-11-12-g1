#include "PlayerJumpState.h"
#include "StatesMachine\Telegram.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "ActionToInput.h"
#include "PhysicController.h"
#include "GameProcess.h"
#include "EngineProcess.h"
#include "Characters\Player\Player.h"
#include "Utils\Random.h"
#include "Core.h"
#include "Base.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Characters\StatesDefs.h"
#include "Logger\Logger.h"


#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerJumpState::CPlayerJumpState( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
	, m_PrevPosition(v3fZERO)
	, m_fJumpYaw(0.f)
	, m_fTime(0.f)
{
	m_pCallback = static_cast<CGameProcess*>(CORE->GetProcess())->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(), _Name);
}

CPlayerJumpState::~CPlayerJumpState()
{

}

void CPlayerJumpState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Jump");
	}
#endif

	CActionToInput *l_pInput = CORE->GetActionToInput();

	//Activa el callback de animación
	m_pCallback->StartAnimation();

	//Hace el salto
	_pCharacter->GetController()->Jump(50.f);

	//Orienta el personaje
	m_fJumpYaw	= _pCharacter->GetYaw();
	if( l_pInput->DoAction("MovePlayerUp") )
	{
		if( l_pInput->DoAction("MovePlayerLeft") )
		{
			m_fJumpYaw	+= FLOAT_PI_VALUE / 4.f;
		}
		else if( l_pInput->DoAction("MovePlayerRight") )
		{
			m_fJumpYaw	-= FLOAT_PI_VALUE / 4.f;
		}
	}
	else if( l_pInput->DoAction("MovePlayerDown") )
	{
		m_fJumpYaw	-= FLOAT_PI_VALUE;

		if( l_pInput->DoAction("MovePlayerLeft") )
		{
			m_fJumpYaw	-= FLOAT_PI_VALUE / 4.f;
		}
		else if( l_pInput->DoAction("MovePlayerRight") )
		{
			m_fJumpYaw	+= FLOAT_PI_VALUE / 4.f;
		}
	}
	else if( l_pInput->DoAction("MovePlayerLeft") )
	{
		m_fJumpYaw	+= FLOAT_PI_VALUE / 2.f;
	}
	else if( l_pInput->DoAction("MovePlayerRight") )
	{
		m_fJumpYaw	-= FLOAT_PI_VALUE / 2.f;
	}

}

void CPlayerJumpState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( m_pCallback->IsAnimationFinished() )
	{
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
	}

	//Movimiento del player hacia adelante
	Vect3f l_Dir = Vect3f( mathUtils::Cos<float>( m_fJumpYaw ), 0.f, mathUtils::Sin<float>( m_fJumpYaw) );

	//Aplica la velocidad al movimiento
	l_Dir = l_Dir * 10.0f * _fElapsedTime;

	//Mueve el controller físico
	_pCharacter->GetController()->Move( l_Dir, _fElapsedTime );

	if( m_PrevPosition.y == _pCharacter->GetController()->GetPosition().y )
	{
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
	}

	//Gira el personaje
	_pCharacter->GetController()->SetYaw( m_fJumpYaw );
	_pCharacter->GetAnimatedModel()->SetYaw( -mathUtils::Rad2Deg(m_fJumpYaw) + 90.f );

	m_fTime = _fElapsedTime;
}

void CPlayerJumpState::OnExit( CCharacter* _pCharacter )
{
	_pCharacter->GetController()->Move( v3fZERO, m_fTime );
}

bool CPlayerJumpState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
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
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerJumpState::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}