#pragma once

#ifndef __ENEMY_CLASS_H__
#define __ENEMY_CLASS_H__

#include <string>
#include "Characters\Character.h"
#include "Math\Vector3.h"
#include <vector>

//---Forward Declarations---
class CRenderManager;
class CCamera;
class CProperties;
class CIdleState;
class CPursuitState;
class CAnimationIdleState;
class CAnimationPursuitState;
class CWayPoint;
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
	void						MoveCharacterToDestination	( Vect3f _Destination, float _ElapsedTime );

private:
	void						UpdateMovement		( float l_ElapsedTime );
	void						Release				( void );

	void						GetWavePoint		();

	//---- Properties ( get & Set )---------------------------------------
public:
	char *						GetName				( void ) const;
	void						SetName				( char * _pName );

	bool						GetMoveAlone		( void ) const										{ return m_bMoverAutomatico; } 
	void						SetMoveAlone		( bool _Valor )										{ m_bMoverAutomatico = _Valor; }

	inline void					SetProperties		( CProperties* _pEnemyProperties )					{ m_pEnemyProperties = _pEnemyProperties; }
	inline CProperties*			GetProperties		( void ) const										{ return m_pEnemyProperties; }

	//---- Members -------------------------------------------------------
protected: 
	bool							m_bIsOk;

private:
	bool							m_bMoverAutomatico;
	bool							m_bLockCamera;

	CWayPoint*						m_DestWavePoint;
	std::vector<Vect3f>				m_WavePoints;
	int								m_CurrentWPIndex;

	CProperties					  *	m_pEnemyProperties;

	CPursuitState				  * m_pPursuitState;
	CIdleState					  * m_pIdleState;
	CAnimationPursuitState		  * m_pAnimationPursuitState;
	CAnimationIdleState			  * m_pAnimationIdleState;	
};
#endif __ENEMY_CLASS_H__
