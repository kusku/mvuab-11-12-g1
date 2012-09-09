#include "DeerDefenseAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerDefenseAnimationState::CDeerDefenseAnimationState( void )
	: CState	("CDeerDefenseAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerDefenseAnimationState::CDeerDefenseAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pDeer	( NULL )
{}


CDeerDefenseAnimationState::~CDeerDefenseAnimationState(void)
{
	if (m_pDeer)
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_DEFENSE_STATE);
		m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
	}
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerDefenseAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerDefenseAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_DEFENSE_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.1f );
}

void CDeerDefenseAnimationState::OnExit( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_DEFENSE_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.1f );
}

bool CDeerDefenseAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
