#include "PlayerAnimationJumpState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationJumpState::CPlayerAnimationJumpState( const std::string &_Name )
	: CState(_Name)
{

}

CPlayerAnimationJumpState::~CPlayerAnimationJumpState()
{

}

void CPlayerAnimationJumpState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("jump");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 1.0f);
}

void CPlayerAnimationJumpState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationJumpState::OnExit( CCharacter* _pCharacter )
{

}

bool CPlayerAnimationJumpState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}