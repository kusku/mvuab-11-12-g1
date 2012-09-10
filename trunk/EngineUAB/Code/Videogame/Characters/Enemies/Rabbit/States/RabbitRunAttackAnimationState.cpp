#include "RabbitRunAttackAnimationState.h"
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
CRabbitRunAttackAnimationState::CRabbitRunAttackAnimationState( void )
	: CState	("CRabbitRunAttackAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitRunAttackAnimationState::CRabbitRunAttackAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{
}

CRabbitRunAttackAnimationState::~CRabbitRunAttackAnimationState( void )
{
	/*if (m_pRabbit) 
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_RUN_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
	}*/
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRunAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
	/*if ( m_CurrentDuration >= m_AnimationDuration )
	{
		int l_iAnimID = m_pRabbit->GetAnimationID(RABBIT_RUN_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);
	}

	m_CurrentDuration += _ElapsedTime;*/
}

void CRabbitRunAttackAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	/*m_CurrentDuration = 0.f;
	m_AnimationDuration = m_pRabbit->GetAnimatedModel()->GetCurrentAnimationDuration(RABBIT_RUN_ATTACK_STATE);*/

	int l_Num = _Character->GetAnimationID(RABBIT_RUN_ATTACK_STATE);
	m_pRabbit->GetAnimatedModel()->ExecuteAction( l_Num, 0.1f );
}

void CRabbitRunAttackAnimationState::OnExit( CCharacter* _Character )
{
	//if ( !m_pRabbit ) 
	//{
	//	// Almacenamos el enemigo
	//	m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	//}

	//int l_Num = m_pRabbit->GetAnimationID(RABBIT_RUN_STATE);
	//m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
}

bool CRabbitRunAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
