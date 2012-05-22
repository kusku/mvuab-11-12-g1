#include "AnimationCaperuIdleState.h"
#include "Characters\Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationCaperuIdleState::CAnimationCaperuIdleState ( void )
//	: CState()
{
}

CAnimationCaperuIdleState::~CAnimationCaperuIdleState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationCaperuIdleState::Execute ( CCharacter* _pCharacter )
{	
	// Aquí trataremos animaciones mientras esta se ejecute...
}

// Se ejecuta cuando el estado es entrado
void CAnimationCaperuIdleState::OnEnter ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->BlendCycle ( 0, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationCaperuIdleState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 0, 0.3f );
}

bool CAnimationCaperuIdleState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
