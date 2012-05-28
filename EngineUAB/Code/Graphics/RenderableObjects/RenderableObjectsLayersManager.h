#pragma once

#ifndef _RENDERABLE_OBJECTS_LAYERS_MANAGER_H
#define _RENDERABLE_OBJECTS_LAYERS_MANAGER_H

class CRenderManager;
class CRenderableObjectsManager;

#include "Utils\TemplatedVectorMapManager.h"
#include "XML\XMLTreeNode.h"
#include <string>

class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager<CRenderableObjectsManager>
{
public:
	CRenderableObjectsLayersManager();
	virtual ~CRenderableObjectsLayersManager();

	void		Destroy		();
	void		Load		( const std::string &FileName );
	void		Reload		();

	void		Update		( float ElapsedTime );
	void		Render		( CRenderManager *RM );
	void		Render		( CRenderManager *RM, const std::string &LayerName );

	static void RegisterMethods();

private:
	void							LoadFile	();
	CRenderableObjectsManager*		GetRenderableObjectManager(CXMLTreeNode &Node);

	std::string						m_FileName;
	CRenderableObjectsManager		*m_pDefaultRenderableObjectManager;
};

#endif
