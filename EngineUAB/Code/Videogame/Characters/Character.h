#pragma once

#include "Math\Plane.h"
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
class CAnimationsStates;
struct Telegram;
//---------------------------//

class CCharacter : public CBaseGameEntity, public CObject3D, public CNamed //, public CMapManager<int>
{
public:
	// ------------- Constructors i Destructors --------------
	CCharacter( int _Id );
	CCharacter( int _Id, const std::string &_Name );
	virtual	~CCharacter();


	static void			RegisterMethods();

	//----Main Functions --------------------------------------
	virtual bool				Init				( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup );
	virtual bool				HandleMessage		( const Telegram& _Msg, bool _Logic = true, bool _Graphic = true  );		// Envia telegramas a las máquinas de estados
	virtual bool				HandleMessage		( const Telegram& _Msg );
	virtual void				Update				( float _ElapsedTime );

	//----Functions -------------------------------------------
	void						MoveController		( const Vect3f &_Dir, float _ElapsedTime );
	
	//----Properties ( get & Set )-----------------------------
	inline CStateMachine<CCharacter>*		GetLogicFSM			( void ) const			{ return m_pLogicStateMachine; }
	inline CStateMachine<CCharacter>*		GetGraphicFSM		( void ) const			{ return m_pGraphicStateMachine; }
	inline CPhysicController*				GetControler		( void ) const			{ return m_pController; }
	inline CAnimatedInstanceModel*			GetAnimatedModel	( void ) const			{ return m_pCurrentAnimatedModel; }


	void						SetPrevPosition		( Vect3f pos )			{ m_PrevPosition = pos; }
	const Vect3f&				GetPrevPosition		( void ) const			{ return m_PrevPosition; }

	// Obtengo el angulo que forma donde mira
	inline Vect3f				GetFront			( void ) const			{ Vect3f l_Front; l_Front.GetXZFromAngle( GetYaw() ) ; return l_Front; }
																			//{ Vect3f front; front.xzFromAngle( m_Yaw ); return front; }

	bool						isPointAtLeft		( const Vect3f &_Position ) const	
																			{
																				const CPlane p( GetPosition( ), GetPosition( ) + GetFront( ), Vect3f( 0.0f, 1.0f, 0.0f ) );
																				return !p.isPointInside( _Position );
																			}

	inline void					SetAnimationsStates	( CAnimationsStates* _pAnimationsStates )			{ m_pAnimationsStates = _pAnimationsStates; }
	inline CAnimationsStates*	GetAnimationsStates	( void ) const										{ return m_pAnimationsStates; }

	//----Members )--------------------------------------------
private:
	bool							m_bIsOk;
	
protected:
	CStateMachine<CCharacter>	  *	m_pLogicStateMachine;		// Part de lógica. Control de la màquina d'estats del personatge
	CStateMachine<CCharacter>	  *	m_pGraphicStateMachine;		// Part de gráfics. Control de la màquina d'estats d'animació del personatge
	CAnimatedInstanceModel		  * m_pCurrentAnimatedModel;	// Part de gráfics. Manté un punté a l'animació en curs

	CAnimationsStates			  *	m_pAnimationsStates;

	CPhysicController			  * m_pController;
	CPhysicUserData				  * m_pPhysicUserDataJugador;

	Vect3f							m_PrevPosition;
};
