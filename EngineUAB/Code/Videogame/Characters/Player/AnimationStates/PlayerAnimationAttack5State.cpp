#include "PlayerAnimationAttack5State.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationAttack5State::CPlayerAnimationAttack5State( CCharacter* _pCharacter,const std::string &_Name )
	: CState(_pCharacter, _Name)
{

}

CPlayerAnimationAttack5State::~CPlayerAnimationAttack5State()
{

}

void CPlayerAnimationAttack5State::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);

	l_iAnimID = _pCharacter->GetAnimationID("attack5");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 0.1f);
}

void CPlayerAnimationAttack5State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationAttack5State::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.3f);
}

bool CPlayerAnimationAttack5State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}