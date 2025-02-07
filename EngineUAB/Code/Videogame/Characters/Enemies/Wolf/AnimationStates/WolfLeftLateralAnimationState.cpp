#include "WolfLeftLateralAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfLeftLateralAnimationState::CWolfLeftLateralAnimationState( void )
	: CState	("CWolfLeftLateralAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfLeftLateralAnimationState::CWolfLeftLateralAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pWolf	( NULL )
{}


CWolfLeftLateralAnimationState::~CWolfLeftLateralAnimationState(void)
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(LEFT_LATERAL_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfLeftLateralAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfLeftLateralAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(LEFT_LATERAL_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfLeftLateralAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(LEFT_LATERAL_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfLeftLateralAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
