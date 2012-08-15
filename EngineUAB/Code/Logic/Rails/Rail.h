//----------------------------------------------------------------------------------
// CRail class
// Author: Marc Cuscullola
//
// Description:
// This class defines a rail in the game.
//----------------------------------------------------------------------------------

#pragma once

#ifndef _RAIL_H_
#define _RAIL_H_

#include "RailDefs.h"
#include "Utils\Named.h"
#include "Math\Vector3.h"
#include <vector>

//--Forward declarations------
class CXMLTreeNode;
class CRenderManager;
class CObject3D;
//----------------------------

class CRail : public CNamed
{
public:
	//--Constructor & Destructor-----------
	CRail();
	CRail( CXMLTreeNode &_Node );
	~CRail();

	//--Basic Methods----------------------
	void				Update			( float _fElapsedTime );

	//--Rail Control Methods---------------
	void				Start			();
	void				Stop			();
	void				Pause			();

	inline bool			HasFinished		() const				{ return m_bHasFinished; }

	//--Property Methods-------------------
	inline void			SetVelocity		( float _velocity )		{ m_fVelocity = _velocity; }
	inline float		GetVelocity		() const				{ return m_fVelocity; }
	
	inline void			SetObject		( CObject3D *_pObject )	{ m_pObject = _pObject; }
	inline CObject3D*	GetObject		() const				{ return m_pObject; }

	inline ERailState	GetState		() const				{ return m_RailState; }

	//--Debug Methods----------------------
	void	Render	( CRenderManager &_RM );

private:
	std::vector<Vect3f>		m_PointVector;

	ERailState				m_RailState;
	float					m_fVelocity;
	bool					m_bHasFinished;
	bool					m_bHasStarted;
	uint16					m_uNextPoint;
	CObject3D				*m_pObject;
};

#endif //_RAIL_H_
