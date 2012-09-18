#include "DeerHitAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerHitAnimationState::CDeerHitAnimationState( CCharacter* _pCharacter )
	: CState	(_pCharacter, "CDeerHitAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerHitAnimationState::CDeerHitAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pDeer	( NULL )
{}


CDeerHitAnimationState::~CDeerHitAnimationState(void)
{
	if ( m_pDeer )
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_HIT_STATE);
		m_pDeer->GetAnimatedModel()->RemoveAction(l_Num);
	}

	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerHitAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerHitAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	/*int l_Num = m_pDeer->GetAnimationID(DEER_IDLE_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle(l_Num, 0.1f);*/

	int l_Num = m_pDeer->GetAnimationID(DEER_HIT_STATE);
	m_pDeer->GetAnimatedModel()->ExecuteAction( l_Num, 0.3f );
}

void CDeerHitAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	if ( m_pDeer )
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_HIT_STATE);
		m_pDeer->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CDeerHitAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
