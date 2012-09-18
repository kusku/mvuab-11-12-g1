#include "PlayerAnimationHitState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationHitState::CPlayerAnimationHitState( CCharacter* _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
{
}

CPlayerAnimationHitState::~CPlayerAnimationHitState()
{

}

void CPlayerAnimationHitState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("hit");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 1.0f);
}

void CPlayerAnimationHitState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationHitState::OnExit( CCharacter* _pCharacter )
{
}

bool CPlayerAnimationHitState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}