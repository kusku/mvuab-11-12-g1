#include "WolfRunAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfRunAnimationState::CWolfRunAnimationState( void )
	: CState	("CWolfRunAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfRunAnimationState::CWolfRunAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pWolf	( NULL )
{
}

CWolfRunAnimationState::~CWolfRunAnimationState( void )
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(RUN_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfRunAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfRunAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(RUN_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfRunAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(RUN_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfRunAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
