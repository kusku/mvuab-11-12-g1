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
CRabbitWalkAnimationState::CRabbitWalkAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitWalkAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitWalkAnimationState::CRabbitWalkAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{
}

CRabbitWalkAnimationState::~CRabbitWalkAnimationState( void )
{
	if (m_pRabbit != NULL)
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_WALK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitWalkAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitWalkAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_WALK_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CRabbitWalkAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( m_pRabbit )
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_WALK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitWalkAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
