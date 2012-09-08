#include "DeerRunAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerRunAnimationState::CDeerRunAnimationState( void )
	: CState	("CDeerRunAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerRunAnimationState::CDeerRunAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pDeer	( NULL )
{
}

CDeerRunAnimationState::~CDeerRunAnimationState( void )
{
	if (m_pDeer != NULL)
	{
		int l_Num = m_pDeer->GetAnimationID(RUN_STATE);
		m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerRunAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerRunAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	/*int l_iAnimID = m_pDeer->GetAnimationID(IDLE_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);*/

	int l_Num = m_pDeer->GetAnimationID(RUN_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.1f );
}

void CDeerRunAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(RUN_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CDeerRunAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
