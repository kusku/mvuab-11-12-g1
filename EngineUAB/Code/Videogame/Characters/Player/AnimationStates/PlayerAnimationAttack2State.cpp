#include "PlayerAnimationAttack2State.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationAttack2State::CPlayerAnimationAttack2State( CCharacter* _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
{

}

CPlayerAnimationAttack2State::~CPlayerAnimationAttack2State()
{

}

void CPlayerAnimationAttack2State::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("attackfinal2");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);

	l_iAnimID = _pCharacter->GetAnimationID("attack2");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 0.1f);
}

void CPlayerAnimationAttack2State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationAttack2State::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("attackfinal2");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.1f);
}

bool CPlayerAnimationAttack2State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}