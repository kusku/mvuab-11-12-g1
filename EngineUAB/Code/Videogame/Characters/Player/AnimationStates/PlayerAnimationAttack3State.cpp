#include "PlayerAnimationAttack3State.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationAttack3State::CPlayerAnimationAttack3State( const std::string &_Name )
	: CState(_Name)
{

}

CPlayerAnimationAttack3State::~CPlayerAnimationAttack3State()
{

}

void CPlayerAnimationAttack3State::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);

	l_iAnimID = _pCharacter->GetAnimationID("attack3");
	_pCharacter->GetAnimatedModel()->ExecuteAction(l_iAnimID, 0.1f);
}

void CPlayerAnimationAttack3State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationAttack3State::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("idle");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.1f);
}

bool CPlayerAnimationAttack3State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}