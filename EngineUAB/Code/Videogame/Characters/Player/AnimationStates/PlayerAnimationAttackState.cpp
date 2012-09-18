#include "PlayerAnimationAttackState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationAttackState::CPlayerAnimationAttackState( CCharacter* _pCharacter,const std::string &_Name )
	: CState(_pCharacter, _Name)
{

}

CPlayerAnimationAttackState::~CPlayerAnimationAttackState()
{

}

void CPlayerAnimationAttackState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("attackfinal1");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);

	l_iAnimID = _pCharacter->GetAnimationID("attack1");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 0.1f);
}

void CPlayerAnimationAttackState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationAttackState::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("attackfinal1");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.1f);
}

bool CPlayerAnimationAttackState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}