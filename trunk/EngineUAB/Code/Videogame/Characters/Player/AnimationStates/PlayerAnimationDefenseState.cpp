#include "PlayerAnimationDefenseState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationDefenseState::CPlayerAnimationDefenseState( CCharacter* _pCharacter,const std::string &_Name )
	: CState(_pCharacter, _Name)
{

}

CPlayerAnimationDefenseState::~CPlayerAnimationDefenseState()
{

}

void CPlayerAnimationDefenseState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("defense");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.2f);
}

void CPlayerAnimationDefenseState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{

}

void CPlayerAnimationDefenseState::OnExit( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("defense");
	_pCharacter->GetAnimatedModel()->ClearCycle(l_iAnimID, 0.2f);
}

bool CPlayerAnimationDefenseState::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	return false;
}