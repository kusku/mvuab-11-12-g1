#include "RabbitDefenseAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfDefenseAnimationState::CWolfDefenseAnimationState( void )
	: CState	("CWolfDefenseAnimationState")
	, m_pRabbit	( NULL )
{
}

CWolfDefenseAnimationState::CWolfDefenseAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CWolfDefenseAnimationState::~CWolfDefenseAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(DEFENSE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfDefenseAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfDefenseAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(DEFENSE_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfDefenseAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(DEFENSE_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfDefenseAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
