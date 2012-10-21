#include "WolfDeathAnimationState.h"
#include "Characters\StatesDefs.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfDeathAnimationState::CWolfDeathAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfDeathAnimationState" )
	, m_pWolf	( NULL )
{
}

CWolfDeathAnimationState::CWolfDeathAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{}


CWolfDeathAnimationState::~CWolfDeathAnimationState(void)
{
	if (m_pWolf)
	{
		/*int l_Num = m_pWolf->GetAnimationID(WOLF_DEATH_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );*/
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfDeathAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfDeathAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_DEATH_STATE);
	m_pWolf->GetAnimatedModel()->ExecuteAction( l_Num, 0.3f, true );
}

void CWolfDeathAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	/*int l_Num = m_pWolf->GetAnimationID(WOLF_DEATH_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );*/
}

bool CWolfDeathAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
