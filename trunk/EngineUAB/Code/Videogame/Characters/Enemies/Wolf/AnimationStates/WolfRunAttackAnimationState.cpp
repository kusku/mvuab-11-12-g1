#include "WolfRunAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfRunAttackAnimationState::CWolfRunAttackAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfRunAttackAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfRunAttackAnimationState::CWolfRunAttackAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{
}

CWolfRunAttackAnimationState::~CWolfRunAttackAnimationState( void )
{
	if (m_pWolf) 
	{
		int l_Num = m_pWolf->GetAnimationID(WOLF_RUN_ATTACK_STATE);
		m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfRunAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfRunAttackAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = _pCharacter->GetAnimationID(WOLF_RUN_ATTACK_STATE);
	m_pWolf->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfRunAttackAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_RUN_ATTACK_STATE);
	m_pWolf->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
}

bool CWolfRunAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
