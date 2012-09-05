#include "RabbitHitAnimationState.h"
#include "Characters\StatesDefs.h"

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfHitAnimationState::CWolfHitAnimationState( void )
	: CState	("CWolfHitAnimationState")
	, m_pRabbit	( NULL )
{
}

CWolfHitAnimationState::CWolfHitAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CWolfHitAnimationState::~CWolfHitAnimationState(void)
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
void CWolfHitAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfHitAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(HIT_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfHitAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(HIT_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfHitAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
