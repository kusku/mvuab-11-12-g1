#include "AnimationIdleState.h"
#include "../Characters/Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationIdleState::CAnimationIdleState ( void )
//	: CState()
{
}

CAnimationIdleState::~CAnimationIdleState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationIdleState::Execute ( CCharacter* _pCharacter )
{	
	// Aquí trataremos animaciones mientras esta se ejecute...
}

// Se ejecuta cuando el estado es entrado
void CAnimationIdleState::OnEnter ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->BlendCycle ( 0, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationIdleState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 0, 0.3f );
}

bool CAnimationIdleState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
