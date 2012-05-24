#include "AnimationCaperuIdleState.h"
#include "Characters\Character.h"

#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "Core.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CAnimationCaperuIdleState::CAnimationCaperuIdleState ( void )
//	: CState()
{
}

CAnimationCaperuIdleState::~CAnimationCaperuIdleState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CAnimationCaperuIdleState::Execute ( CCharacter* _pCharacter )
{	
	// Aquí trataremos animaciones mientras esta se ejecute...
}

// Se ejecuta cuando el estado es entrado
void CAnimationCaperuIdleState::OnEnter ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
	_pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationCaperuIdleState::OnExit ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "idle" );
	_pCharacter->GetAnimatedModel()->ClearCycle ( i, 0.3f );
}

bool CAnimationCaperuIdleState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
