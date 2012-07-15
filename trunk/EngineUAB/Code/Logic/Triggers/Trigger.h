#pragma once

#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include "Utils\Named.h"
#include "Math\Vector3.h"
#include "Math\Color.h"

//--Forward Declarations------
class CRenderManager;
class CObject3D;
class CPhysicActor;
class CXMLTreeNode;
//----------------------------

class CTrigger : public CNamed
{
public:
	//---Constructor & Destructor------------
	CTrigger();
	~CTrigger();

	//---Basic Methods-----------------------
	void			CleanUp		();
	virtual void	Init		();
	virtual void	ReadData	( CXMLTreeNode &_Node ) = 0;
	virtual void	Update		( float _fElapsedTime ) = 0;
	virtual void	Render		( CRenderManager *_RM ) = 0;

	//---Trigger Methods---------------------
	virtual void	OnEnter		() = 0;
	virtual void	OnExecute	() = 0;
	virtual void	OnExit		() = 0;

	//---Get & Set Methods-------------------
	void			SetRenderDebug		( bool render )				{ m_bRenderDebugTrigger = render; }
	void			SetActive			( bool active )				{ m_bIsActive = active; }
	void			SetPosition			( const Vect3f & pos )		{ m_Position = pos; }
	void			SetTriggerObject	( CObject3D * val )			{ m_pTriggerObject = val; }
	void			SetTriggerActor		( CPhysicActor * actor )	{ m_pTriggerActor = actor; }
	void			SetRenderColor		( const CColor & color )	{ m_RenderColor = color; }

	bool			IsRenderDebug		() const					{ return m_bRenderDebugTrigger; }
	bool			IsActive			() const					{ return m_bIsActive; }
	const Vect3f&	GetPosition			() const					{ return m_Position; }
	CObject3D *		GetTriggerObject	() const					{ return m_pTriggerObject; }
	CPhysicActor *	GetTriggerActor		() const					{ return m_pTriggerActor; }
	const CColor&	GetRenderColor		() const					{ return m_RenderColor; }

protected:
	Vect3f		m_Position;
	CColor		m_RenderColor;

	bool		m_bRenderDebugTrigger;
	bool		m_bIsActive;

	CObject3D	*m_pTriggerObject;
	CPhysicActor *m_pTriggerActor;
};

#endif //_TRIGGER_H_
