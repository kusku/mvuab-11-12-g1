#include "DeerIdleAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerIdleAnimationState::CDeerIdleAnimationState( void )
	: CState	("CDeerIdleAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerIdleAnimationState::CDeerIdleAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pDeer	( NULL )
{}


CDeerIdleAnimationState::~CDeerIdleAnimationState(void)
{
	if (m_pDeer)
	{
		int l_Num = m_pDeer->GetAnimationID(IDLE_STATE);
		m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerIdleAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(IDLE_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CDeerIdleAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerIdleAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(IDLE_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CDeerIdleAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
