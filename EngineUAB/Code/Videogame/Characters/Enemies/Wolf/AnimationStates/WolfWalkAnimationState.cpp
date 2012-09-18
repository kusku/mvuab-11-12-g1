#include "WolfWalkAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfWalkAnimationState::CWolfWalkAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfWalkAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfWalkAnimationState::CWolfWalkAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{
}

CWolfWalkAnimationState::~CWolfWalkAnimationState( void )
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(WOLF_WALK_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfWalkAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfWalkAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_WALK_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfWalkAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_WALK_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfWalkAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
