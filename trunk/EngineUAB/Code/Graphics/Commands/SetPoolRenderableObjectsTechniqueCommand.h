#pragma once

#ifndef _SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_COMMAND_H
#define _SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;

class CSetPoolRenderableObjectsTechniqueCommand : public CSceneRendererCommand
{
public:
	CSetPoolRenderableObjectsTechniqueCommand(CXMLTreeNode &Node);
	
	void	Execute		( CRenderManager &RM );

private:
	CPoolRenderableObjectTechnique* m_PoolRenderableObjectTechnique;

};

#endif


