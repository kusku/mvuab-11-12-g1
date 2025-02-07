#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"
#include "Characters\StatesDefs.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitHitAnimationState::CRabbitHitAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitHitAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitHitAnimationState::CRabbitHitAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{}


CRabbitHitAnimationState::~CRabbitHitAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_HIT_STATE);
		m_pRabbit->GetAnimatedModel()->RemoveAction(l_Num);
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitHitAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitHitAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	/*int l_Num = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle(l_Num, 0.1f);*/

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_HIT_STATE);
	m_pRabbit->GetAnimatedModel()->ExecuteAction( l_Num, 0.1f );
}

void CRabbitHitAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( m_pRabbit )
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_HIT_STATE);
		m_pRabbit->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CRabbitHitAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
