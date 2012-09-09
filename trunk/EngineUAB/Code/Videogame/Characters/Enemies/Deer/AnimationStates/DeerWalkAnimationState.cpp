#include "DeerWalkAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerWalkAnimationState::CDeerWalkAnimationState( void )
	: CState	("CDeerWalkAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerWalkAnimationState::CDeerWalkAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pDeer	( NULL )
{
}

CDeerWalkAnimationState::~CDeerWalkAnimationState( void )
{
	if (m_pDeer != NULL)
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_WALK_STATE);
		m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerWalkAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerWalkAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_WALK_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CDeerWalkAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_WALK_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CDeerWalkAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
