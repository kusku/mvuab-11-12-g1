#include "RabbitIdle2AnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfIdle2AnimationState::CWolfIdle2AnimationState( void )
	: CState	("CWolfIdle2AnimationState")
	, m_pRabbit	( NULL )
{
}

CWolfIdle2AnimationState::CWolfIdle2AnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CWolfIdle2AnimationState::~CWolfIdle2AnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(HIT_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfIdle2AnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfIdle2AnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(IDLE2_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfIdle2AnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(IDLE2_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfIdle2AnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
