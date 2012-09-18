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
CDeerRunAnimationState::CDeerRunAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CDeerRunAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerRunAnimationState::CDeerRunAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pDeer	( NULL )
{
}

CDeerRunAnimationState::~CDeerRunAnimationState( void )
{
	if (m_pDeer != NULL)
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_RUN_STATE);
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

void CDeerRunAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	/*int l_iAnimID = m_pDeer->GetAnimationID(DEER_IDLE_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.1f);*/

	int l_Num = m_pDeer->GetAnimationID(DEER_RUN_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.1f );
}

void CDeerRunAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_RUN_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CDeerRunAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
