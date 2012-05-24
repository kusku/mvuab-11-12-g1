#include "AnimationWolfWalkState.h"
#include "Characters\Character.h"

#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "Core.h"

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
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "walk" );
	_pCharacter->GetAnimatedModel()->BlendCycle( i , 0.3f );  //(*l_Vect).at(2), 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationWolfWalkState::OnExit ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "walk" );
	_pCharacter->GetAnimatedModel()->ClearCycle( i , 0.3f );  //(*l_Vect).at(2), 0.3f );
}

bool CAnimationWolfWalkState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
