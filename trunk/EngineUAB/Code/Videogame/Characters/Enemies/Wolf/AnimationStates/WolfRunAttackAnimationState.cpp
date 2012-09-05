#include "RabbitRunAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfRunAttackAnimationState::CWolfRunAttackAnimationState( void )
	: CState	("CWolfRunAttackAnimationState")
	, m_pRabbit	( NULL )
{
}

CWolfRunAttackAnimationState::CWolfRunAttackAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{
}

CWolfRunAttackAnimationState::~CWolfRunAttackAnimationState( void )
{
	if (m_pRabbit) 
	{
		int l_Num = m_pRabbit->GetAnimationID(RUN_ATTACK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfRunAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfRunAttackAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = _Character->GetAnimationID(RUN_ATTACK_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfRunAttackAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(RUN_ATTACK_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfRunAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
