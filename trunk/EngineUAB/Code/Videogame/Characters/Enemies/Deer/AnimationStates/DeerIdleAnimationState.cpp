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
CDeerIdleAnimationState::CDeerIdleAnimationState( CCharacter* _pCharacter )
	: CState	(_pCharacter,"CDeerIdleAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerIdleAnimationState::CDeerIdleAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter,_Name)
	, m_pDeer	( NULL )
{}


CDeerIdleAnimationState::~CDeerIdleAnimationState(void)
{
	if (m_pDeer)
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_IDLE_STATE);
		m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerIdleAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_IDLE_STATE);
	m_pDeer->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
}

void CDeerIdleAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerIdleAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_IDLE_STATE);
	m_pDeer->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
}

bool CDeerIdleAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
