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

#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "RenderableObject.h"

class CRenderManager;
class CEffectTechnique;
class CXMLTreeNode;

class CRenderableObjectsManager : public CMapManager<CRenderableObject>
{
public:
	CRenderableObjectsManager();
	virtual ~CRenderableObjectsManager();

	void		Update		( float elapsedTime );
	void		Render		( CRenderManager *RM );

	CRenderableObject*	AddMeshInstance			( const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position );
	CRenderableObject*	AddMeshInstance			( CXMLTreeNode &Node );
	CRenderableObject*	AddAnimatedMeshInstance	( const std::string &Name, const Vect3f &Position );
	CRenderableObject*	AddAnimatedMeshInstance ( CXMLTreeNode &Node );
	bool				AddResource				( const std::string &Name, CRenderableObject *RenderableObject );
	
	CRenderableObject*	GetInstance				( const std::string &Name );

	void		CleanUp		();
	void		Load		( const std::string &FileName );		
	void		Reload		();

protected:
	void		LoadFile		();

	std::vector<CRenderableObject*>			m_RenderableObjects;
	std::string								m_FileName;
};

#endif
