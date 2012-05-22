#include "AnimationWolfWalkState.h"
#include "Characters\Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationWolfWalkState::CAnimationWolfWalkState ( void )
//	: CState()
{
}

CAnimationWolfWalkState::~CAnimationWolfWalkState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationWolfWalkState::Execute ( CCharacter* _pCharacter )
{	
	// Aquí trataremos animaciones mientras esta se ejecute...
}

// Se ejecuta cuando el estado es entrado
void CAnimationWolfWalkState::OnEnter ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->BlendCycle ( 1, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationWolfWalkState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 1, 0.3f );
}

bool CAnimationWolfWalkState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
