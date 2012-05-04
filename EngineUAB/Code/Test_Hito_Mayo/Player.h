#pragma once

#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>

#include "Object3D.h"
#include "Math\Vector3.h"
#include "Utils\Named.h"

class CRenderManager;
class CCamera;


class CPlayer : public CObject3D, public CNamed
{
public:
	//--- Init and End protocols------------------------------------------
			CPlayer				( void );
			CPlayer				( std::string _Name );
			~CPlayer			( void );

	void	Done				( void );
	bool	IsOK				( void ) const { return m_bIsOk; };
	bool	Init				( void );

	//---- Main Functions ------------------------------------------------
	void	Update				( float _ElapsedTime ); //, CCamera* _pCamera );
	void	Render				( CRenderManager *_RM);
	//void	RenderScene			( void );

	//---- Functions -----------------------------------------------------
	void	MoverAutomaticamente( float _ElapsedTime );
	void	MoverManualmente	( float _ElapsedTime );

private:
	void	UpdateInputActions	( float _ElapsedTime ); //, CCamera* _pCamera );		NO CAL!! en principi el player té associada la càmera o al revés. A més, per commutar 1a 3a camera no serviria
	void	Release				( void );

	//---- Properties ( get & Set )---------------------------------------
public:
	char *	GetName				( void ) const;
	void	SetName				( char * _pName );

	bool	GetMoveAlone		( void ) const			{ return m_bMoverAutomatico; } 
	void	SetMoveAlone		( bool _Valor )			{ m_bMoverAutomatico = _Valor; }

	Vect3f	GetDirection		( void ) const			{ return m_vDirection; }

	//---- Members -------------------------------------------------------
protected: 
	bool			m_bIsOk;

private:
	short			m_sNumeroVides;
	bool			m_bMoverAutomatico;
	bool			m_bLockCamera;

	Vect3f			m_vDirection;

};

#endif
