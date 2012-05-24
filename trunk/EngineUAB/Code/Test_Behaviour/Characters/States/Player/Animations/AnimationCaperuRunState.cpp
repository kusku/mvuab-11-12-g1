#include "AnimationCaperuRunState.h"
#include "Characters\Character.h"
//#include "Characters\Player\Player.h"
//#include "Characters\States\AnimationsStates.h"
//#include "Characters\States\AnimationsStatesManager.h"

#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "Core.h"

#include <vector>

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationCaperuRunState::CAnimationCaperuRunState ( void )
	//: CState()
{}


CAnimationCaperuRunState::~CAnimationCaperuRunState ( void )
{
}

//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationCaperuRunState::Execute	( CCharacter* _pCharacter )
{
	// Aquí trataremos animaciones mientras esta se ejecute...
}


// Se ejecuta cuando el estado es entrado
void CAnimationCaperuRunState::OnEnter ( CCharacter* _pCharacter )
{
	//CPlayer * l_Player = dynamic_cast<CPlayer*>  (_pCharacter);
	
	//CAnimationsStates* l_Anim = l_Player->GetAnimationsStates();
	//std::vector<uint16>* l_Vect = l_Player->GetAnimationsStates()->GetResource( "run" );
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "run" );
	_pCharacter->GetAnimatedModel()->BlendCycle( i , 0.3f );  //(*l_Vect).at(2), 0.3f );

}

	// Se ejecuta cuando el estado sale
void CAnimationCaperuRunState::OnExit ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "run" );
	_pCharacter->GetAnimatedModel()->ClearCycle ( i, 0.3f );
}

bool CAnimationCaperuRunState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
