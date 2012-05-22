#include "AnimationCaperuAttackState.h"
#include "Characters\Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationCaperuAttackState::CAnimationCaperuAttackState ( void )
//	: CState()
{
}

CAnimationCaperuAttackState::~CAnimationCaperuAttackState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationCaperuAttackState::Execute ( CCharacter* _pCharacter )
{	
	// Aquí trataremos animaciones mientras esta se ejecute...
}

// Se ejecuta cuando el estado es entrado
void CAnimationCaperuAttackState::OnEnter ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->BlendCycle ( 1, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationCaperuAttackState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 1, 0.3f );
}

bool CAnimationCaperuAttackState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
