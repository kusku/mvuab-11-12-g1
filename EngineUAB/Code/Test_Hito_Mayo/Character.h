#pragma once

#include "Utils\MapManager.h"
#include "Object3D.h"
#include "Utils\Named.h"

#include "Base.h"
#include "Core.h"

#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

//--- Foward Declarations ---//
//class CState;
class CIdleState;
class CPursuitState;
class CPlayerControler;
struct Telegram;

//---------------------------//

class CCharacter : public CBaseGameEntity, public CObject3D, public CNamed //, public CMapManager<int>
{
public:
	// ------------- Constructors i Destructors --------------
					CCharacter			( int _Id );
					CCharacter			( int _Id, const std::string &_Name );
	virtual			~CCharacter			( void );


	//----Main Functions --------------------------------------
	virtual bool	Init				( void );
	virtual bool	HandleMessage		( const Telegram& _Msg );
	virtual void	Update				( float _ElapsedTime );

	//----Functions -------------------------------------------


	//----Properties ( get & Set )-----------------------------
	CStateMachine<CCharacter>*	GetFSM				( void ) const			{ return m_pStateMachine; }
	CPlayerControler*			GetControler		( void ) const			{ return m_pControler; }
	CAnimatedInstanceModel*		GetAnimatedModel	( void ) const			{ return m_pCurrentAnimatedModel; }

	//----Members )--------------------------------------------
private:
	bool							m_bIsOk;
	
protected:
	CStateMachine<CCharacter>	  *	m_pStateMachine;			// Part de lógica. Control de la màquina d'estats del personatge
	CPlayerControler			  * m_pControler;				// Part de física. Control del player en el escenari
	CAnimatedInstanceModel		  * m_pCurrentAnimatedModel;	// Part de gráfics. Manté un punté a l'animació en curs

	CPursuitState				  * m_pPursuitState;
	CIdleState					  * m_pIdleState;

	int								m_State;
	int								m_Life;
};

