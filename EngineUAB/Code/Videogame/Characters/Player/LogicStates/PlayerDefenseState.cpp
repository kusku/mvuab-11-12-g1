#include "PlayerDefenseState.h"
#include "StatesMachine\Telegram.h"
#include "PhysicController.h"
#include "ActionToInput.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "Core.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerDefenseState::CPlayerDefenseState( const std::string &_Name )
	: CState(_Name)
{

}

CPlayerDefenseState::~CPlayerDefenseState()
{

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
	if( !CORE->GetActionToInput()->DoAction("DefensePlayer") )
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
	return false;
}