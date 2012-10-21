#include "RabbitStillAttackAnimationState.h"
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
CRabbitStillAttackAnimationState::CRabbitStillAttackAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitStillAttackAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitStillAttackAnimationState::CRabbitStillAttackAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{
}

CRabbitStillAttackAnimationState::~CRabbitStillAttackAnimationState( void )
{
	if ( m_pRabbit )
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_STILL_ATTACK_STATE);
		m_pRabbit->GetAnimatedModel()->RemoveAction(l_Num);
	}

	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitStillAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitStillAttackAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	/*int l_iAnimID = m_pRabbit->GetAnimationID(RABBIT_WALK_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.3f);*/

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_STILL_ATTACK_STATE);
	m_pRabbit->GetAnimatedModel()->ExecuteAction( l_Num, 0.3f, true );
}

void CRabbitStillAttackAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	if ( m_pRabbit ) 
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_STILL_ATTACK_STATE);
		m_pRabbit->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CRabbitStillAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
