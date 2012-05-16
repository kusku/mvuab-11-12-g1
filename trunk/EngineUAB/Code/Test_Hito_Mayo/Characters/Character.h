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
	virtual			~CCharacter			( void );


	//----Main Functions --------------------------------------
	virtual bool	Init				( void );
	virtual bool	HandleMessage		( const Telegram& _Msg );
	virtual void	Update				( float _ElapsedTime );

	//----Functions -------------------------------------------
	void			MoveController		( const Vect3f &_Dir, float _ElapsedTime );

	//----Properties ( get & Set )-----------------------------
	CStateMachine<CCharacter>*	GetFSM				( void ) const			{ return m_pLogicStateMachine; }
	CAnimatedInstanceModel*		GetAnimatedModel	( void ) const			{ return m_pCurrentAnimatedModel; }

	void						SetPrevPosition		( Vect3f pos )			{ m_PrevPosition = pos; }
	const Vect3f&				GetPrevPosition		( void ) const			{ return m_PrevPosition; }

	//----Members )--------------------------------------------
private:
	bool							m_bIsOk;
	
protected:
	CStateMachine<CCharacter>		*m_pLogicStateMachine;			// Part de lógica. Control de la màquina d'estats del personatge
	CAnimatedInstanceModel			*m_pCurrentAnimatedModel;	// Part de gráfics. Manté un punté a l'animació en curs

	CPhysicController				*m_pController;
	CPhysicUserData					*m_pPhysicUserDataJugador;

	CPursuitState					*m_pPursuitState;
	CIdleState						*m_pIdleState;

	int								m_State;
	int								m_Life;

	Vect3f							m_PrevPosition;
};

