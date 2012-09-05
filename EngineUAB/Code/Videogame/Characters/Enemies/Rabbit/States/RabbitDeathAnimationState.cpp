#include "RabbitDeathAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDeathAnimationState::CRabbitDeathAnimationState( void )
	: CState	( "CRabbitDeathAnimationState" )
	, m_pRabbit	( NULL )
{
}

CRabbitDeathAnimationState::CRabbitDeathAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CRabbitDeathAnimationState::~CRabbitDeathAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(DEATH_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDeathAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitDeathAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(DEATH_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CRabbitDeathAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(DEATH_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CRabbitDeathAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
