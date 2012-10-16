#include "PlayerDefenseState.h"
#include "StatesMachine\Telegram.h"
#include "PhysicController.h"
#include "ActionToInput.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "GameProcess.h"
#include "Characters/Player/Player.h"
#include "SoundManager.h"
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
}

void CPlayerDefenseState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( !m_pInput->DoAction("DefensePlayer") )
	{
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
	}

	_pCharacter->GetController()->Move( v3fZERO, _fElapsedTime);
}

void CPlayerDefenseState::OnExit( CCharacter* _pCharacter )
{
}

bool CPlayerDefenseState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Caperucita_combo_hit");


	// TODO:: HACER SONIDO DE BLOQUEO!
	if( _Message.Msg == Msg_Attack )
	{
		return true;
	}
	else if( _Message.Msg == Msg_Push )
	{
		/*CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		
		sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
		_pCharacter->MoveTo2(l_Info->Direccion * 1.2f, l_Info->ElapsedTime);
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerDefenseState::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}