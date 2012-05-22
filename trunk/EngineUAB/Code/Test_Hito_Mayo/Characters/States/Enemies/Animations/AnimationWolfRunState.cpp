#include "AnimationWolfRunState.h"
#include "Characters\Character.h"
#include "Characters\Player\Player.h"
#include "Characters\States\AnimationsStatesManager.h"
#include "Characters\States\AnimationsStates.h"

#include <vector>

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationWolfRunState::CAnimationWolfRunState ( void )
	//: CState()
{}


CAnimationWolfRunState::~CAnimationWolfRunState ( void )
{
}

//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationWolfRunState::Execute	( CCharacter* _pCharacter )
{
	// Aquí trataremos animaciones mientras esta se ejecute...
}


// Se ejecuta cuando el estado es entrado
void CAnimationWolfRunState::OnEnter ( CCharacter* _pCharacter )
{
	CPlayer * l_Player = dynamic_cast<CPlayer*>  (_pCharacter);
	
	CAnimationsStates* l_Anim = l_Player->GetAnimationsStates();
	std::vector<uint16>* l_Vect = l_Player->GetAnimationsStates()->GetResource( "run" );
	l_Player->GetAnimatedModel()->BlendCycle ( 2, 0.3f );  //(*l_Vect).at(2), 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationWolfRunState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 2, 0.3f );
}

bool CAnimationWolfRunState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
