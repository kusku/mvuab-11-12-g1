#include "DeerStillAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerStillAttackAnimationState::CDeerStillAttackAnimationState( void )
	: CState	("CDeerStillAttackAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerStillAttackAnimationState::CDeerStillAttackAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pDeer	( NULL )
{
}

CDeerStillAttackAnimationState::~CDeerStillAttackAnimationState( void )
{
	if ( m_pDeer )
	{
		/*int l_Num = m_pDeer->GetAnimationID(DEER_WALK_STATE);
		m_pDeer->GetAnimatedModel()->ClearCycle(l_Num, 0.3f);*/
	}

	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerStillAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerStillAttackAnimationState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	/*int l_iAnimID = m_pDeer->GetAnimationID(DEER_WALK_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.3f);*/

	int l_Num = m_pDeer->GetAnimationID(DEER_STILL_ATTACK_STATE);
	m_pDeer->GetAnimatedModel()->ExecuteAction( l_Num, 0.1f );
}

void CDeerStillAttackAnimationState::OnExit( CCharacter* _Character )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}
	
	/*int l_Num = m_pDeer->GetAnimationID(DEER_WALK_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle(l_Num, 0.3f);*/
}

bool CDeerStillAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
