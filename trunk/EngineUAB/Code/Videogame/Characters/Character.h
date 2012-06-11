#pragma once

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Math\Plane.h"
#include "Utils\MapManager.h"
#include "Object3D.h"
#include "Utils\Named.h"
#include "Base.h"
#include "Core.h"
#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\State.h"
#include "StatesMachine\StateMachine.h"
#include "PhysicsDefs.h"
#include <string>

//--- Foward Declarations ---//
class CProperties;
class CPhysicController;
class CPhysicUserData;
class CAnimationsStates;
class CAnimatedInstanceModel;
struct Telegram;
//---------------------------//

class CCharacter : public CBaseGameEntity, public CObject3D, public CNamed //, public CMapManager<int>
{
public:
	// ------------- Constructors i Destructors --------------
	CCharacter();
	CCharacter( const std::string &_Name );
	CCharacter( int _ID, const std::string &_Name );
	virtual	~CCharacter();

	//----Main Functions --------------------------------------
	virtual bool		Initialize			( const std::string &_Name, const Vect3f &_InitialPosicion, ECollisionGroup _Grup );
	virtual bool		Init				();
	virtual bool		HandleMessage		( const Telegram& _Msg, bool _Logic = true, bool _Graphic = true  );		// Envia telegramas a las máquinas de estados
	virtual bool		HandleMessage		( const Telegram& _Msg );
	virtual void		Update				( float _ElapsedTime );

	//----Functions -------------------------------------------
	void						MoveController		( const Vect3f &_Dir, float _ElapsedTime );
	void						MoveTo				( const Vect3f &_Position, float _ElapsedTime );
	void						FaceTo				( const Vect3f &_Position, float _ElapsedTime );
	//bool						IsPlayerDetected	( void );
	
	void						AddLife				( int _Life );								
	void						RestLife			( int _Life );								
	void						AddSpeed			( int _Speed );								
	void						RestSpeed			( int _Speed );								
	void						AddStrong			( int _Strong );								
	void						RestStrong			( int _Strong );								


	//----Properties ( get & Set )-----------------------------
	inline CStateMachine<CCharacter>*		GetLogicFSM			() const			{ return m_pLogicStateMachine; }
	inline CStateMachine<CCharacter>*		GetGraphicFSM		() const			{ return m_pGraphicStateMachine; }
	inline CPhysicController*				GetController		() const			{ return m_pController; }
	inline CAnimatedInstanceModel*			GetAnimatedModel	() const			{ return m_pCurrentAnimatedModel; }
	inline CPhysicUserData*					GetPhysicUserData	() const			{ return m_pPhysicUserDataJugador; }
	int										GetAnimationID		( const std::string &_AnimationName ) const;

	inline void					SetPrevPosition		( Vect3f pos )			{ m_PrevPosition = pos; }
	inline const Vect3f&		GetPrevPosition		( void ) const				{ return m_PrevPosition; }

	void						SetEnable			( bool enable );
	inline bool					IsEnable			( void ) const				{ return m_bIsEnable; }


	inline void					SetLocked			( bool locked )			{ m_bLocked = locked; }
	inline bool					GetLocked			( void ) const				{ return m_bLocked; }

	// Obtengo el angulo que forma donde mira
	inline Vect3f				GetFront			( void ) const			{ Vect3f l_Front; l_Front.GetXZFromAngle( GetYaw() ) ; return l_Front; }
																			//{ Vect3f front; front.xzFromAngle( m_Yaw ); return front; }

	bool						IsPointAtLeft		( const Vect3f &_Position ) const	
																			{
																				const CPlane p( GetPosition( ), GetPosition( ) + GetFront( ), Vect3f( 0.0f, 1.0f, 0.0f ) );
																				return !p.isPointInside( _Position );
																			}

	inline void					SetAnimationsStates	( CAnimationsStates* _pAnimationsStates )	{ m_pAnimationsStates = _pAnimationsStates; }
	inline CAnimationsStates*	GetAnimationsStates	( void ) const								{ return m_pAnimationsStates; }

	inline void					SetProperties		( CProperties* _pProperties )				{ m_pProperties = _pProperties; }
	inline CProperties*			GetProperties		( void ) const								{ return m_pProperties; }


	//----Members ---------------------------------------------
private:
	bool			m_bIsOk;
	bool			m_bLocked;
	bool			m_bIsEnable;

protected:
	CStateMachine<CCharacter>	  *	m_pLogicStateMachine;		// Part de lógica. Control de la màquina d'estats del personatge
	CStateMachine<CCharacter>	  *	m_pGraphicStateMachine;		// Part de gráfics. Control de la màquina d'estats d'animació del personatge
	CAnimatedInstanceModel		  * m_pCurrentAnimatedModel;	// Part de gráfics. Manté un punté a l'animació en curs

	CAnimationsStates			  *	m_pAnimationsStates;
	CProperties					  *	m_pProperties;

	CPhysicController			  * m_pController;
	CPhysicUserData				  * m_pPhysicUserDataJugador;
	
	Vect3f							m_PrevPosition;
};

#endif