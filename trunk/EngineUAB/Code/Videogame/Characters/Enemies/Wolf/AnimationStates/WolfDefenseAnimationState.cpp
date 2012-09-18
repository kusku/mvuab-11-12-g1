#include "WolfDefenseAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfDefenseAnimationState::CWolfDefenseAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfDefenseAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfDefenseAnimationState::CWolfDefenseAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{}


CWolfDefenseAnimationState::~CWolfDefenseAnimationState(void)
{
	if (m_pWolf)
	{
		int l_Num = m_pWolf->GetAnimationID(WOLF_DEFENSE_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfDefenseAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfDefenseAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_DEFENSE_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.1f );
}

void CWolfDefenseAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_DEFENSE_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
}

bool CWolfDefenseAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
