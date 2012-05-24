#include "AnimationCaperuAttackState.h"
#include "Characters\Character.h"

#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "Core.h"

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
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "attack" );
	_pCharacter->GetAnimatedModel()->BlendCycle ( i, 0.3f );
}

	// Se ejecuta cuando el estado sale
void CAnimationCaperuAttackState::OnExit ( CCharacter* _pCharacter )
{
	CAnimatedCoreModel * l_Core =  _pCharacter->GetAnimatedModel()->GetAnimatedCoreModel();
	int i = l_Core->GetCoreModel()->getCoreAnimationId ( "attack" );
	_pCharacter->GetAnimatedModel()->ClearCycle ( i, 0.3f );
}

bool CAnimationCaperuAttackState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
