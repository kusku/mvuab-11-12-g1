#include "PlayerAnimationRunState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationRunState::CPlayerAnimationRunState( const std::string &_Name )
	: CState(_Name)
{

}

CPlayerAnimationRunState::~CPlayerAnimationRunState()
{

}

void CPlayerAnimationRunState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("run");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.3f);
}

void CPlayerAnimationRunState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationRunState::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("run");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.3f);
}

bool CPlayerAnimationRunState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}