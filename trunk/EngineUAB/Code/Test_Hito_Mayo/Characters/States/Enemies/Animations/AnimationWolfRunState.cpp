#include "AnimationWolfRunState.h"
#include "Characters\Character.h"
//#include "Characters\Enemy\Enemy.h"
//#include "Characters\States\AnimationsStatesManager.h"
//#include "Characters\States\AnimationsStates.h"

#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "Core.h"

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
	//CEnemy * l_Enemy = dynamic_cast<CEnemy*> (_pCharacter);
	
	//CAnimationsStates* l_Anim = l_Enemy->GetAnimationsStates();
	//std::vector<uint16>* l_Vect = l_Enemy->GetAnimationsStates()->GetResource( "run" );
	//l_Enemy->GetAnimatedModel()->BlendCycle ( 1, 0.3f );  //(*l_Vect).at(2), 0.3f );

	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "run" );
	_pCharacter->GetAnimatedModel()->BlendCycle( i , 0.3f );  //(*l_Vect).at(2), 0.3f );

}

	// Se ejecuta cuando el estado sale
void CAnimationWolfRunState::OnExit ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "run" );
	_pCharacter->GetAnimatedModel()->ClearCycle( i , 0.3f );  //(*l_Vect).at(2), 0.3f );
}

bool CAnimationWolfRunState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
