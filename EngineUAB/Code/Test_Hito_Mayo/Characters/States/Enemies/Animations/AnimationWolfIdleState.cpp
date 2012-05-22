#include "AnimationWolfIdleState.h"
#include "Characters\Character.h"

#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "Core.h"

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
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
	_pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationWolfIdleState::OnExit ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
	_pCharacter->GetAnimatedModel()->ClearCycle ( i, 0.3f );
}

bool CAnimationWolfIdleState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
