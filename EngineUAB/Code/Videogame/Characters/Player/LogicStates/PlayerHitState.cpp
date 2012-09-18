#include "PlayerHitState.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Player\Player.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "ActionToInput.h"
#include "Callbacks/Animation/AnimationCallback.h"
#include "Callbacks/Animation/AnimationCallbackManager.h"
#include "PhysicController.h"
#include "EngineProcess.h"
#include "GameProcess.h"
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

}

void CPlayerHitState::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Hit");
	}
#endif

	static_cast<CPlayer*>(_pCharacter)->HitToPlayer();

	m_pCallback->StartAnimation();
}

void CPlayerHitState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( m_pCallback->IsAnimationFinished() )
	{	
		_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
		_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
	}
	
	_pCharacter->GetController()->Move( v3fZERO, _fElapsedTime );
}

void CPlayerHitState::OnExit( CCharacter* _pCharacter )
{
}

bool CPlayerHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}