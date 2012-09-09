#include "RabbitHitAnimationState.h"
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
CRabbitHitAnimationState::CRabbitHitAnimationState( void )
	: CState	("CRabbitHitAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitHitAnimationState::CRabbitHitAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CRabbitHitAnimationState::~CRabbitHitAnimationState(void)
{
	if (m_pRabbit)
	{
		/*int l_Num = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );*/
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitHitAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitHitAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	/*int l_Num = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle(l_Num, 0.1f);*/

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_HIT_STATE);
	m_pRabbit->GetAnimatedModel()->ExecuteAction( l_Num, 0.1f );
}

void CRabbitHitAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	/*int l_Num = m_pRabbit->GetAnimationID(RABBIT_HIT_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );*/
}

bool CRabbitHitAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
