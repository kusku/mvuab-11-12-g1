#include "RabbitIdleAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitIdleAnimationState::CRabbitIdleAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitIdleAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitIdleAnimationState::CRabbitIdleAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{}


CRabbitIdleAnimationState::~CRabbitIdleAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitIdleAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CRabbitIdleAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitIdleAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if (m_pRabbit) 
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitIdleAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
