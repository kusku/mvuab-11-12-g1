#include "WolfImpactRunAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfImpactRunAttackAnimationState::CWolfImpactRunAttackAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CWolfImpactRunAttackAnimationState")
	, m_pWolf	( NULL )
{
}

CWolfImpactRunAttackAnimationState::CWolfImpactRunAttackAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pWolf	( NULL )
{
}

CWolfImpactRunAttackAnimationState::~CWolfImpactRunAttackAnimationState( void )
{
	if (m_pWolf) 
	{
		int l_Num = m_pWolf->GetAnimationID(WOLF_ONLY_IMPACT_RUN_ATTACK_STATE);
		m_pWolf->GetAnimatedModel()->RemoveAction( l_Num );
	}
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfImpactRunAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfImpactRunAttackAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = _pCharacter->GetAnimationID(WOLF_ONLY_IMPACT_RUN_ATTACK_STATE);
	m_pWolf->GetAnimatedModel()->ExecuteAction( l_Num, 0.1f );
}

void CWolfImpactRunAttackAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pWolf ) 
	{
		// Almacenamos el enemigo
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	int l_Num = m_pWolf->GetAnimationID(WOLF_ONLY_IMPACT_RUN_ATTACK_STATE);
	m_pWolf->GetAnimatedModel()->RemoveAction( l_Num );
}

bool CWolfImpactRunAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
