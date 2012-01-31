#pragma once

#ifndef _SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_COMMAND_H
#define _SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;

class CSetPoolRenderableObjectsTechniqueCommand : public CSceneRendererCommand
{
private:
	CPoolRenderableObjectTechnique* m_PoolRenderableObjectTechnique;

public:
	CSetPoolRenderableObjectsTechniqueCommand();
	CSetPoolRenderableObjectsTechniqueCommand(CXMLTreeNode &Node);
	virtual ~CSetPoolRenderableObjectsTechniqueCommand();

	void	Execute		( CRenderManager &RM );


};

#endif


