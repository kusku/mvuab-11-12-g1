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
CWolfDeathAnimationState::CWolfDeathAnimationState( void )
	: CState	( "CWolfDeathAnimationState" )
	, m_pRabbit	( NULL )
{
}

CWolfDeathAnimationState::CWolfDeathAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CWolfDeathAnimationState::~CWolfDeathAnimationState(void)
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
void CWolfDeathAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CWolfDeathAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(DEATH_STATE);
	m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CWolfDeathAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CWolf*> (_Character);
	}

	int l_Num = m_pRabbit->GetAnimationID(DEATH_STATE);
	m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CWolfDeathAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
