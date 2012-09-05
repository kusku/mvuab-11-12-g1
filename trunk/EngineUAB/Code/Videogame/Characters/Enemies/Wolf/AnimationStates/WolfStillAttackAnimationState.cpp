#include "RabbitStillAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfStillAttackAnimationState::CWolfStillAttackAnimationState( void )
	: CState	("CWolfStillAttackAnimationState")
	, m_pRabbit	( NULL )
{
}

CWolfStillAttackAnimationState::CWolfStillAttackAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{
}

CWolfStillAttackAnimationState::~CWolfStillAttackAnimationState( void )
{
	if ( m_pRabbit )
	{	
		int l_Num = m_pRabbit->GetAnimationID(STILL_ATTACK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfStillAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfStillAttackAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = _Character->GetAnimationID(STILL_ATTACK_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfStillAttackAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(STILL_ATTACK_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfStillAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
