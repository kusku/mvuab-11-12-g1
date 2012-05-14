#include "PursuitState.h"
#include "../Characters/Character.h"

//--------------------------------------------------
//				CONSTRUCTORS/DESTRUCTORS
//--------------------------------------------------

CPursuitState::CPursuitState ( void )
	//: CState()
{}


CPursuitState::~CPursuitState ( void )
{
}

//--------------------------------------------------
//				FUNCTIONS PRINCIPALS
//--------------------------------------------------
void CPursuitState::Execute	( CCharacter* _pCharacter )
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
void CPursuitState::OnEnter ( CCharacter* _pCharacter )
{

}

	// Se ejecuta cuando el estado sale
void CPursuitState::OnExit ( CCharacter* _pCharacter )
{

}

bool CPursuitState::OnMessage ( CCharacter* _pCharacter, const Telegram& )
{
	return true;
}

//--------------------------------------------------
//				PROPERTIES
//--------------------------------------------------
