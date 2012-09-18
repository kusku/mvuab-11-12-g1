#include "RabbitDefenseAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDefenseAnimationState::CRabbitDefenseAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitDefenseAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitDefenseAnimationState::CRabbitDefenseAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{}


CRabbitDefenseAnimationState::~CRabbitDefenseAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_DEFENSE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDefenseAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitDefenseAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_DEFENSE_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.1f );
}

void CRabbitDefenseAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( m_pRabbit )
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_DEFENSE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
	}
}

bool CRabbitDefenseAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
