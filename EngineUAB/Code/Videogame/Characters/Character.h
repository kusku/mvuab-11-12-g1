#pragma once

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Math\Plane.h"
#include "Utils\MapManager.h"
#include "Object3D.h"
#include "Utils\Named.h"
#include "Utils\Active.h"
#include "Base.h"
#include "Core.h"
#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\State.h"
#include "StatesMachine\StateMachine.h"
#include "PhysicsDefs.h"
#include "Properties/Properties.h"
#include <string>
#include "Characters\CharactersDefs.h"

//--- Foward Declarations ---//
class CPhysicController;
class CPhysicUserData;
class CAnimationsStates;
class CAnimatedInstanceModel;
class CSteeringBehaviors;
class CSteeringEntity;
class CSpeaker;
struct STelegram;
//---------------------------//

class CCharacter : public CBaseGameEntity, public CObject3D
{
public:
	// ------------- Constructors i Destructors --------------
								CCharacter					( void );
								CCharacter					( const std::string &_Name );
								CCharacter					( int _ID, const std::string &_Name );
	virtual						~CCharacter					( void );

	//----Main Functions --------------------------------------
	virtual bool				Initialize					( const std::string &_Name, const std::string &_Core, const Vect3f &_InitialPosicion, ECollisionGroup _Grup );
	bool						InitializeAI				( void );
	
	virtual bool				Init						( void ) = 0; // Inicialización del caracter en cuestión, de los hijos.
	
	virtual bool				HandleMessage				( const STelegram& _Msg, bool _Logic = true, bool _Graphic = true  );		// Envia telegramas a las máquinas de estados
	virtual bool				HandleMessage				( const STelegram& _Msg );
	virtual void				Update						( float _ElapsedTime );
	virtual void				UpdatePlayer				( float _ElapsedTime );
	virtual void				UpdateIA					( float _ElapsedTime );

	//----Methods ---------------------------------------------
	void						MoveController				( const Vect3f &_Dir, float _ElapsedTime );
	void						MoveCharacter				( const Vect3f &_Pos );
	void						MoveTo						( const Vect3f &_Position, float _ElapsedTime );
	void						MoveTo2						( const Vect3f &_Position, float _ElapsedTime );
	void						FaceTo						( const Vect3f &_Position, float _ElapsedTime );
	void						FaceTo2						( const Vect3f &_Position, float _ElapsedTime );
	void						FaceToForPlayer				( const Vect3f &_Position, float _ElapsedTime );
	
	bool						AddLife						( int _Life );								
	virtual void				RestLife					( int _Life );								
	void						AddSpeed					( float _Speed );								
	void						RestSpeed					( float _Speed );								
	void						AddStrong					( int _Strong );								
	void						RestStrong					( int _Strong );								

	CPhysicUserData*			ShootCollisionRay			( void );

	inline bool					IsPlayerInsideDistance		( float _Distance );
	bool						IsPlayerInsideImpactDistance( void );
	bool						IsPlayerDetected			( void );
	bool						IsPlayerChased				( void );
	bool						IsPlayerAtacable			( void );
	bool						IsPlayerReady				( void );
	bool						IsPlayerReached				( void );
	bool						IsObstacleVisibleInAngle	( CCharacter * _Obstacle, float _Angle );

	bool						IsEnemyFocused				( void );
	bool						IsEnemyPreparedToAttack		( void );
	bool						IsEnemyAproximatedToAttack	( void );
	
	Vect3f						GetPointInsideCameraFrustum	( float _VisionAngle ) const;
	Vect3f						GetPositionToAttack			( void ) const;
	void						GoIntoCameraFrustum			( float _RangeAngle, float _ElapsedTime );	// Permite colocar un caracter dentro del frustum de la càmara en unos grados desde el centro
	bool						IsEnemyIntoCameraFrustum	( float _RangeAngle, float _ElapsedTime );	// Permite ver si un caracter está dentro del frustum de la càmara en unos grados
	bool						IsCollisionedWithSomething	( void );
						
	float						GetDistanceToPlayer			( void );

	// Permite cargar los estados 
	/*virtual void				LoadGraphicStates		( void ) = 0; 
	virtual void				LoadLogicStates			( void ) = 0;*/

	void						Appearance					( void );
	
	virtual void				SetSoundsOff				( void );

	//----Properties ( get & Set )-----------------------------
	inline CStateMachine<CCharacter>*		GetLogicFSM			( void ) const			{ return m_pLogicStateMachine; }
	inline CStateMachine<CCharacter>*		GetGraphicFSM		( void ) const			{ return m_pGraphicStateMachine; }
	inline CPhysicController*				GetController		( void ) const			{ return m_pController; }
	inline CAnimatedInstanceModel*			GetAnimatedModel	( void ) const			{ return m_pCurrentAnimatedModel; }
	inline CPhysicUserData*					GetPhysicUserData	( void ) const			{ return m_pPhysicUserDataJugador; }
	int										GetAnimationID		( const std::string &_AnimationName );

	/*inline void					SetPrevPosition		( const Vect3f &pos )				{ m_pSteeringEntity->SetPreviousPosition(pos); }
	inline Vect3f				GetPrevPosition		( void ) const						{ return m_pSteeringEntity->GetPreviousPosition(); }*/

	void						SetEnable				( bool _Enable );			
	inline bool					IsEnable				( void ) const						{ return this->GetProperties()->GetActive(); }  // m_bIsEnable; }

	inline void					SetAlive				( bool alive )						{ m_bIsAlive = alive; }
	inline bool					IsAlive					( void ) const						{ return m_bIsAlive; }

	inline void					SetLocked				( bool locked )						{ m_bLocked = locked; }
	inline bool					GetLocked				( void ) const						{ return m_bLocked; }

	inline CSteeringBehaviors*	GetBehaviors			( void ) const						{ return m_pBehaviors; }
	inline CSteeringEntity*		GetSteeringEntity		( void ) const						{ return m_pSteeringEntity; }
	

	// Obtengo el angulo que forma donde mira
	inline Vect3f				GetFront			( void ) const						{ Vect3f l_Front; l_Front.GetXZFromAngle( GetYaw() ) ; return l_Front; }

	inline bool					IsPointAtLeft		( const Vect3f &_Position, const Vect3f &_Front ) const	
														{
															const CPlane p( GetPosition(), GetPosition() + _Front, v3fY );
															return !p.isPointInside( _Position );
														}

	inline void					SetAnimationsStates		( CAnimationsStates* _pAnimationsStates )	{ m_pAnimationsStates = _pAnimationsStates; }
	inline CAnimationsStates*	GetAnimationsStates		( void ) const								{ return m_pAnimationsStates; }

	inline void					SetProperties			( CProperties* _pProperties )				{ m_pProperties = _pProperties; }
	inline CProperties*			GetProperties			( void ) const								{ return m_pProperties; }

	inline void					SetAvalaibleToAttack	( bool _isReady )							{ m_ReadyToAttack = _isReady; }
	inline bool					GetAvalaibleToAttack	( void ) const								{ return m_ReadyToAttack; }

	CState<CCharacter>*			GetLogicState			( const std::string &_State );
	CState<CCharacter>*			GetAnimationState		( const std::string &_State );
	
	CCharacter *				GetPlayer				( void );

	virtual void				BeDead					( void ) = 0;

	inline void					SetPlayerHasBeenReached ( bool _PlayerReached ) 					{ m_PlayerHasBeenReached =_PlayerReached; }
	inline bool					GetPlayerHasBeenReached ( void ) const								{ return m_PlayerHasBeenReached; }

	inline void					SetToBeTired			( bool _IsTired ) 							{ m_IsTired =_IsTired; }
	inline bool					GetIsTired				( void ) const								{ return m_IsTired; }

	inline const std::string&	GetSpeakerName			( void ) const								{ return m_SpeakerName; }

	inline void					SetCharacterType		( eCharacterTypes _Type ) 					{ m_Type = _Type; }
	inline eCharacterTypes		GetCharacterType		( void ) const								{ return m_Type; }

	//----Members ---------------------------------------------
private:
	bool							m_bIsOk;
	bool							m_bIsAlive;

protected:
	typedef CState< CCharacter >			TState;
	typedef std::map<std::string, TState* >	TStateMap;

	TStateMap						m_LogicStatesMap;
	TStateMap						m_AnimationStatesMap;
	
	CStateMachine<CCharacter>	  *	m_pLogicStateMachine;		// Part de lógica. Control de la màquina d'estats del personatge
	CStateMachine<CCharacter>	  *	m_pGraphicStateMachine;		// Part de gráfics. Control de la màquina d'estats d'animació del personatge
	CAnimatedInstanceModel		  * m_pCurrentAnimatedModel;	// Part de gráfics. Manté un punté a l'animació en curs

	CAnimationsStates			  *	m_pAnimationsStates;
	CProperties					  *	m_pProperties;

	CPhysicController			  * m_pController;
	CPhysicUserData				  * m_pPhysicUserDataJugador;
	
	CSteeringBehaviors			  * m_pBehaviors;
	CSteeringEntity				  * m_pSteeringEntity;

	CSpeaker					  * m_pSpeaker;

	bool							m_ReadyToAttack; 
	bool							m_bLocked;

	bool							m_PlayerHasBeenReached;		// Indica si acaba de ser alcanzado el player
	bool							m_IsTired;					// Indica si estamo cansados

	std::string						m_SpeakerName;

	eCharacterTypes					m_Type;
};

#endif