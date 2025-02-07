#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>

#include "Characters\Character.h"

#include "Math\Vector3.h"


//---Forward Declarations---
class CRenderManager;
class CCamera;
class CProperties;
class CIdleState;
class CPursuitState;
class CAnimationCaperuIdleState;
class CAnimationCaperuRunState;
class CAnimationCaperuAttackState;
//--------------------------

class CPlayer : public CCharacter
{
public:
	//--- Init and End protocols------------------------------------------
				CPlayer				( void );
				CPlayer				( const std::string &_Name );
				~CPlayer			( void );

	void		Done				( void );
	bool		IsOK				( void ) const { return m_bIsOk; };
	bool		Init				( void );

	//---- Main Functions ------------------------------------------------
	void		Update				( float _ElapsedTime, CCamera *_Camera ); //, CCamera* _pCamera );
	void		Render				( CRenderManager *_RM);

	//---- Functions -----------------------------------------------------
	void		MoverManualmente	( float _ElapsedTime );

private:
	void		UpdateInputActions	( float _ElapsedTime, CCamera* _pCamera );		//NO CAL!! en principi el player t� associada la c�mera o al rev�s. A m�s, per commutar 1a 3a camera no serviria
	void		Release				( void );

	//---- Properties ( get & Set )---------------------------------------
public:
	char *		GetName				( void ) const;
	void		SetName				( char * _pName );

	void		SetLockCamera		( bool lock )			{ m_bLockCamera = lock; }
	bool		GetLockCamera		( void ) const			{ return m_bLockCamera; }

	Vect3f		GetDirection		( void ) const			{ return m_vDirection; }

	inline void					SetProperties		( CProperties* _pPlayerProperties )					{ m_pPlayerProperties = _pPlayerProperties; }
	inline CProperties*			GetProperties		( void ) const										{ return m_pPlayerProperties; }

	
	
	//---- Members -------------------------------------------------------
protected: 
	bool				m_bIsOk;

private:
	short				m_sNumeroVides;
	bool				m_bMoverAutomatico;
	bool				m_bLockCamera;
	
	Vect3f				m_vDirection;
	
	CProperties					  *	m_pPlayerProperties;

	CPursuitState				  * m_pPursuitState;
	CIdleState					  * m_pIdleState;

	CAnimationCaperuIdleState	  * m_pAnimationIdleState;
	CAnimationCaperuRunState	  * m_pAnimationRunState;
	CAnimationCaperuAttackState	  * m_pAnimationAttackState;
};

#endif __PLAYER_H__
