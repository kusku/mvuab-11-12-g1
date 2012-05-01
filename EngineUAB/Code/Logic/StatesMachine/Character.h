#pragma once

#include "Base.h"
#include "Core.h"

#include "BaseGameEntity.h"
#include "StateMachine.h"
#include "IdleState.h"
#include "PursuitState.h"

#include "State.h"

//--- Foward Declarations ---//
//class CState;
struct Telegram;
//---------------------------//

class CCharacter : public CBaseGameEntity
{
public:
	// ------------- Constructors i Destructors --------------
					CCharacter			( int _Id );
	virtual			~CCharacter			( void );


	//----Main Functions --------------------------------------

	virtual bool	HandleMessage		( const Telegram& _Msg );
	void			Update				( float _ElapsedTime );

	//----Functions -------------------------------------------


	//----Properties ( get & Set )-----------------------------
	CStateMachine<CCharacter>* GetFSM	( void ) const			{ return m_pStateMachine; }


	//----Members )--------------------------------------------
private:
	CStateMachine<CCharacter>*		m_pStateMachine;

	int			m_Life;
};

