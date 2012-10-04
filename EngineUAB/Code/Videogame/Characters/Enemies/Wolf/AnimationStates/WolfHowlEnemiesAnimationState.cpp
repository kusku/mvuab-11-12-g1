#include "WolfHowlEnemiesAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfHowlEnemiesAnimationState::CWolfHowlEnemiesAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfHowlEnemiesAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfHowlEnemiesAnimationState::CWolfHowlEnemiesAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{
}

CWolfHowlEnemiesAnimationState::~CWolfHowlEnemiesAnimationState( void )
{
	if ( m_pWolf )
	{	
		int l_Num = m_pWolf->GetAnimationID(WOLF_HOWL_ENEMIES_STATE);
		m_pWolf->GetAnimatedModel()->RemoveAction( l_Num );
	}
	
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfHowlEnemiesAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfHowlEnemiesAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = _pCharacter->GetAnimationID(WOLF_HOWL_ENEMIES_STATE);
	m_pWolf->GetAnimatedModel()->ExecuteAction( l_Num, 0.3f );
}

void CWolfHowlEnemiesAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_HOWL_ENEMIES_STATE);
	m_pWolf->GetAnimatedModel()->RemoveAction(l_Num);
}

bool CWolfHowlEnemiesAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
