#include "RabbitRunAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitRunAnimationState::CRabbitRunAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitRunAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitRunAnimationState::CRabbitRunAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{
}

CRabbitRunAnimationState::~CRabbitRunAnimationState( void )
{
	if (m_pRabbit != NULL)
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_RUN_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRunAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitRunAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	/*int l_iAnimID = m_pRabbit->GetAnimationID(RABBIT_IDLE_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);*/

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_RUN_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.1f );
}

void CRabbitRunAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( m_pRabbit ) 
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_RUN_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitRunAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
