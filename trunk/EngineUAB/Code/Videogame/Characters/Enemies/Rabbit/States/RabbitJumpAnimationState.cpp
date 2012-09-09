#include "RabbitJumpAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitJumpAnimationState::CRabbitJumpAnimationState( void )
	: CState	("CRabbitJumpAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitJumpAnimationState::CRabbitJumpAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CRabbitJumpAnimationState::~CRabbitJumpAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_JUMP_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitJumpAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitJumpAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_JUMP_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CRabbitJumpAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_JUMP_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CRabbitJumpAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
