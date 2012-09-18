#include "WolfJumpAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfJumpAnimationState::CWolfJumpAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfJumpAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfJumpAnimationState::CWolfJumpAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{}


CWolfJumpAnimationState::~CWolfJumpAnimationState(void)
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(WOLF_JUMP_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfJumpAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfJumpAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_JUMP_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfJumpAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_JUMP_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfJumpAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
