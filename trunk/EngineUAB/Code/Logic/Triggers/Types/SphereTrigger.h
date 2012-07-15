#pragma once

#ifndef _SPHERE_TRIGGER_H_
#define _SPHERE_TRIGGER_H_

#include "Triggers\Trigger.h"

//--Forward Declarations------
class CRenderManager;
class CXMLTreeNode;
//----------------------------

class CSphereTrigger : public CTrigger
{
public:
	//---Constructor & Destructor------------
	CSphereTrigger();
	~CSphereTrigger();

	//---Basic Methods-----------------------
	void	ReadData	( CXMLTreeNode &_Node );
	void	Init		();
	void	Update		( float _fElapsedTime );
	void	Render		( CRenderManager *_RM );

	//---Trigger Methods---------------------
	void	OnEnter		();
	void	OnStay		();
	void	OnExit		();

	//---Get & Set Methods-------------------
	void	SetRadius	( float _fRadius )		{ m_fRadius = _fRadius; }
	float	GetRadius	() const				{ return m_fRadius; }

private:
	float		m_fRadius;

};

#endif //_SPHERE_TRIGGER_H_
