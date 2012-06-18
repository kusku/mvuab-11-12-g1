//----------------------------------------------------------------------------------
// CRenderableObjectsManager class
// Author: Marc Cuscullola
//
// Description:
// Gestiona todos los Renderable Objects
//----------------------------------------------------------------------------------
#pragma once

#ifndef _RENDERABLEOBJECTSMANAGER_H
#define _RENDERABLEOBJECTSMANAGER_H

#include <vector>
#include <string>
#include <map>

#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "RenderableObject.h"

class CRenderManager;
class CEffectTechnique;
class CXMLTreeNode;
class CInstanceMeshHW;
class CObject3D;

class CRenderableObjectsManager : public CMapManager<CRenderableObject>
{

protected:
	std::vector<CRenderableObject*>			m_RenderableObjects;
	std::string								m_FileName;

	typedef std::map<std::string, CInstanceMeshHW*> InstanceMeshHWMap;
	typedef InstanceMeshHWMap::iterator InstanceMeshHWMapIt;
	typedef InstanceMeshHWMap::const_iterator InstanceMeshHWMapItConst;

	InstanceMeshHWMap						m_InstanceMeshHWMap;

public:
	CRenderableObjectsManager();
	virtual ~CRenderableObjectsManager();

	void		Update		( float elapsedTime );
	void		Render		( CRenderManager *RM );

	CRenderableObject*	AddMeshInstance			( const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position );
	CRenderableObject*	AddMeshInstance			( CXMLTreeNode &Node );
	bool				AddMeshInstanceHw		( CXMLTreeNode &Node );
	CRenderableObject*	AddAnimatedMeshInstance	( const std::string &Name, const Vect3f &Position );
	CRenderableObject*	AddAnimatedMeshInstance ( CXMLTreeNode &Node );
	bool				AddResource				( const std::string &Name, CRenderableObject *RenderableObject );
	
	CRenderableObject*	GetInstance				( const std::string &Name );
	CObject3D*			GetInstanceHW			( const std::string &Name );

	TMapResource&		GetResourceMap			()
	{
		return m_Resources;
	}

	InstanceMeshHWMap&	GetResourceHWMap		()
	{
		return m_InstanceMeshHWMap;
	}

	void				CleanUp		();
};

#endif
