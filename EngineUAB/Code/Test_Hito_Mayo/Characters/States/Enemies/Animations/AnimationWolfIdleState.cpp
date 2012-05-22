#include "AnimationWolfIdleState.h"
#include "Characters\Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationWolfIdleState::CAnimationWolfIdleState ( void )
//	: CState()
{
}

CAnimationWolfIdleState::~CAnimationWolfIdleState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationWolfIdleState::Execute ( CCharacter* _pCharacter )
{	
	// Aquí trataremos animaciones mientras esta se ejecute...
}

// Se ejecuta cuando el estado es entrado
void CAnimationWolfIdleState::OnEnter ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->BlendCycle ( 0, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationWolfIdleState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 0, 0.3f );
}

bool CAnimationWolfIdleState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
