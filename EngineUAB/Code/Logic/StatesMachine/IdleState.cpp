#include "IdleState.h"
#include "Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CIdleState::CIdleState ( void )
	: CState()
{
}

CIdleState::~CIdleState ( void )
{
}


//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CIdleState::Execute ( CCharacter* _pCharacter )
{	
//	/*if (troll->isSafe())
//	{
//		troll->ChangeState(new State_Sleep());
//	}
//	else
//	{
//		troll->MoveAwayFromEnemy();
//	}*/
}


// Se ejecuta cuando el estado es entrado
void CIdleState::OnEnter ( CCharacter* _pCharacter )
{

}

	// Se ejecuta cuando el estado sale
void CIdleState::OnExit ( CCharacter* _pCharacter )
{

}



//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
