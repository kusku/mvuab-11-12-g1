#include "WolfHitAnimationState.h"
#include "Characters\StatesDefs.h"

#include "Characters\Enemies\Wolf\Wolf.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfHitAnimationState::CWolfHitAnimationState( void )
	: CState	("CWolfHitAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfHitAnimationState::CWolfHitAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pWolf	( NULL )
{}


CWolfHitAnimationState::~CWolfHitAnimationState(void)
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(HIT_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfHitAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfHitAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(HIT_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfHitAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(HIT_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfHitAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
