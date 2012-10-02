#include "PlayerAnimationAttack6State.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationAttack6State::CPlayerAnimationAttack6State( CCharacter* _pCharacter,const std::string &_Name )
	: CState(_pCharacter, _Name)
{

}

CPlayerAnimationAttack6State::~CPlayerAnimationAttack6State()
{

}

void CPlayerAnimationAttack6State::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);

	l_iAnimID = _pCharacter->GetAnimationID("attack6");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 0.1f);
}

void CPlayerAnimationAttack6State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationAttack6State::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.3f);
}

bool CPlayerAnimationAttack6State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}