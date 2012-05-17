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

#include "PhysicsDefs.h"

//--- Foward Declarations ---//
//class CState;
class CPhysicController;
class CPhysicUserData;
class CIdleState;
class CPursuitState;
struct Telegram;
//---------------------------//

class CCharacter : public CBaseGameEntity, public CObject3D, public CNamed //, public CMapManager<int>
{
public:
	// ------------- Constructors i Destructors --------------
								CCharacter			( int _Id );
								CCharacter			( int _Id, const std::string &_Name );
	virtual						~CCharacter			( void );


	//----Main Functions --------------------------------------
	virtual bool				Init				( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup );
	virtual bool				HandleMessage		( const Telegram& _Msg, bool _Logic = true, bool _Graphic = true  );		// Envia telegramas a las máquinas de estados
	virtual bool				HandleMessage		( const Telegram& _Msg );
	virtual void				Update				( float _ElapsedTime );

	//----Functions -------------------------------------------
	void						MoveController		( const Vect3f &_Dir, float _ElapsedTime );

	//----Properties ( get & Set )-----------------------------
	virtual inline CStateMachine<CCharacter>*		GetLogicFSM			( void ) const			{ return m_pLogicStateMachine; }
	virtual inline CStateMachine<CCharacter>*		GetGraphicFSM		( void ) const			{ return m_pGraphicStateMachine; }
	virtual inline CPhysicController*				GetControler		( void ) const			{ return m_pController; }
	virtual inline CAnimatedInstanceModel*			GetAnimatedModel	( void ) const			{ return m_pCurrentAnimatedModel; }


	void						SetPrevPosition		( Vect3f pos )			{ m_PrevPosition = pos; }
	const Vect3f&				GetPrevPosition		( void ) const			{ return m_PrevPosition; }

	//----Members )--------------------------------------------
private:
	bool							m_bIsOk;
	
protected:
	CStateMachine<CCharacter>	  *	m_pLogicStateMachine;		// Part de lógica. Control de la màquina d'estats del personatge
	CStateMachine<CCharacter>	  *	m_pGraphicStateMachine;		// Part de gráfics. Control de la màquina d'estats d'animació del personatge
	CAnimatedInstanceModel		  * m_pCurrentAnimatedModel;	// Part de gráfics. Manté un punté a l'animació en curs

	CPhysicController			  * m_pController;
	CPhysicUserData				  * m_pPhysicUserDataJugador;

	Vect3f							m_PrevPosition;
};

