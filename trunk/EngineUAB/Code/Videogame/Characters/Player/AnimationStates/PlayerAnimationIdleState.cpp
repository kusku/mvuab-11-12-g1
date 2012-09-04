#include "PlayerAnimationIdleState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationIdleState::CPlayerAnimationIdleState( const std::string &_Name )
	: CState(_Name)
{

}

CPlayerAnimationIdleState::~CPlayerAnimationIdleState()
{

}

void CPlayerAnimationIdleState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 1.0f);
}

void CPlayerAnimationIdleState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationIdleState::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.3f);
}

bool CPlayerAnimationIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}