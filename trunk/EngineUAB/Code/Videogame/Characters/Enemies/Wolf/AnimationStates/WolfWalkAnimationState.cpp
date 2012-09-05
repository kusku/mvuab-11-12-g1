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
CWolfWalkAnimationState::CWolfWalkAnimationState( void )
	: CState	("CWolfWalkAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfWalkAnimationState::CWolfWalkAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pWolf	( NULL )
{
}

CWolfWalkAnimationState::~CWolfWalkAnimationState( void )
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(WALK_STATE);
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

void CWolfWalkAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(WALK_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfWalkAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(WALK_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfWalkAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
