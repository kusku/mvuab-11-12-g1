#pragma once

#ifndef __ENEMY_CLASS_H__
#define __ENEMY_CLASS_H__

#include <string>
#include "Character.h"
#include "Math\Vector3.h"

//---Forward Declarations---
class CRenderManager;
class CCamera;
class CProperties;
class CAnimationsStates;
class CIdleState;
class CPursuitState;
class CAnimationIdleState;
class CAnimationPursuitState;
//--------------------------

class CEnemy : public CCharacter
{
public:
	//--- Init and End protocols------------------------------------------
								CEnemy				( int _ID );
								CEnemy				( int _ID, const std::string &_Name );
								~CEnemy				( void );

	void						Done				( void );
	bool						IsOK				( void ) const { return m_bIsOk; };
	bool						Init				( void );

	//---- Main Functions ------------------------------------------------
	void						Update				( float _ElapsedTime ); //, CCamera* _pCamera );
	void						Render				( CRenderManager *_RM);
	bool						HandleMessage		( const Telegram& _Msg, bool _Logic, bool _Graphic );
	//void						RenderScene			( void );

	//---- Functions -----------------------------------------------------
	void						MoverAutomaticamente( float _ElapsedTime );
	void						MoverManualmente	( float _ElapsedTime );

private:
	//void						UpdateInputActions	( float _ElapsedTime ); //, CCamera* _pCamera ); 	NO CAL!! en principi el player té associada la càmera o al revés. A més, per commutar 1a 3a camera no serviria
	void						Release				( void );

	//---- Properties ( get & Set )---------------------------------------
public:
	char *						GetName				( void ) const;
	void						SetName				( char * _pName );

	bool						GetMoveAlone		( void ) const										{ return m_bMoverAutomatico; } 
	void						SetMoveAlone		( bool _Valor )										{ m_bMoverAutomatico = _Valor; }

	inline void					SetProperties		( CProperties* _pEnemyProperties )					{ m_pEnemyProperties = _pEnemyProperties; }
	inline CProperties*			GetProperties		( void ) const										{ return m_pEnemyProperties; }

	inline void					SetAnimationsStates	( CAnimationsStates* _pEnemyAnimationsStates )		{ m_pEnemyAnimationsStates = _pEnemyAnimationsStates; }
	inline CAnimationsStates*	GetAnimationsStates	( void ) const										{ return m_pEnemyAnimationsStates; }

	//---- Members -------------------------------------------------------
protected: 
	bool							m_bIsOk;

private:
	bool							m_bMoverAutomatico;
	bool							m_bLockCamera;

	CProperties					  *	m_pEnemyProperties;
	CAnimationsStates			  *	m_pEnemyAnimationsStates;

	CPursuitState				  * m_pPursuitState;
	CIdleState					  * m_pIdleState;
	CAnimationPursuitState		  * m_pAnimationPursuitState;
	CAnimationIdleState			  * m_pAnimationIdleState;	
};
#endif __ENEMY_CLASS_H__
