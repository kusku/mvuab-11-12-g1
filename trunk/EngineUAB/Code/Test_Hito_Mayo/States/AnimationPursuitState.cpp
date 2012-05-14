#include "AnimationPursuitState.h"
#include "../Characters/Character.h"
#include "../Characters/Player.h"
#include "AnimationsStatesManager.h"
#include "AnimationsStates.h"

#include <vector>

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationPursuitState::CAnimationPursuitState ( void )
	//: CState()
{}


CAnimationPursuitState::~CAnimationPursuitState ( void )
{
}

//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationPursuitState::Execute	( CCharacter* _pCharacter )
{
	// Aquí trataremos animaciones mientras esta se ejecute...
}


// Se ejecuta cuando el estado es entrado
void CAnimationPursuitState::OnEnter ( CCharacter* _pCharacter )
{
	CPlayer * l_Player = dynamic_cast<CPlayer*>  (_pCharacter);
	
	CAnimationsStates* l_Anim = l_Player->GetAnimationsStates();
	std::vector<uint16>* l_Vect = l_Player->GetAnimationsStates()->GetResource( "run" );
	l_Player->GetAnimatedModel()->BlendCycle ( (*l_Vect).at(2), 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationPursuitState::OnExit ( CCharacter* _pCharacter )
{
	_pCharacter->GetAnimatedModel()->ClearCycle ( 1, 0.3f );
}

bool CAnimationPursuitState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
