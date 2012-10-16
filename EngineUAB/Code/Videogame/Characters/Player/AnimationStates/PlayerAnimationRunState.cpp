#include "PlayerAnimationRunState.h"
#include "StatesMachine\Telegram.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "SoundManager.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAnimationRunState::CPlayerAnimationRunState( CCharacter* _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
{
	m_AnimationDuration = _pCharacter->GetAnimatedModel()->GetCurrentAnimationDuration("run");
	m_ActionCallback = new CActionStateCallback(0, m_AnimationDuration);
}

CPlayerAnimationRunState::~CPlayerAnimationRunState()
{
	CHECKED_DELETE(m_ActionCallback);
}

void CPlayerAnimationRunState::OnEnter( CCharacter* _pCharacter )
{
	int l_iAnimID = _pCharacter->GetAnimationID("run");
	_pCharacter->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.3f);

	m_ActionCallback->InitAction();
	m_ActionCallback->StartAction();
}

void CPlayerAnimationRunState::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	m_ActionCallback->Update(_fElapsedTime);

	if ( m_ActionCallback->IsActionStarted() )
	{
		if ( m_ActionCallback->IsActionInTime(0.20f) && !m_ActionCallback->IsActionInTime(0.30f) )
		{
			CORE->GetSoundManager()->PlayEvent("Play_EFX_Caperucita_fast_steps");
		}
		
		if ( m_ActionCallback->IsActionInTime(0.55f) && !m_ActionCallback->IsActionInTime(0.65f) )
		{
			CORE->GetSoundManager()->PlayEvent("Play_EFX_Caperucita_fast_steps");
		}
	}
	
	if ( m_ActionCallback->IsActionFinished() )
	{
		m_ActionCallback->InitAction();
		m_ActionCallback->StartAction();
	}
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