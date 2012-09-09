#include "WolfStillAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfStillAttackAnimationState::CWolfStillAttackAnimationState( void )
	: CState	("CWolfStillAttackAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfStillAttackAnimationState::CWolfStillAttackAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pWolf	( NULL )
{
}

CWolfStillAttackAnimationState::~CWolfStillAttackAnimationState( void )
{
	if ( m_pWolf )
	{	
		int l_Num = m_pWolf->GetAnimationID(WOLF_STILL_ATTACK_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfStillAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfStillAttackAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = _Character->GetAnimationID(WOLF_STILL_ATTACK_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfStillAttackAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_STILL_ATTACK_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfStillAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
