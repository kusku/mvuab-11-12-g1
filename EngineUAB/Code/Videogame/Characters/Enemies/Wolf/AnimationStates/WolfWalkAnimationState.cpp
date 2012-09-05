#include "RabbitWalkAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfWalkAnimationState::CWolfWalkAnimationState( void )
	: CState	("CWolfWalkAnimationState")
	, m_pRabbit	( NULL )
{
}

CWolfWalkAnimationState::CWolfWalkAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{
}

CWolfWalkAnimationState::~CWolfWalkAnimationState( void )
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(WALK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfWalkAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfWalkAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(WALK_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfWalkAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(WALK_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfWalkAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
