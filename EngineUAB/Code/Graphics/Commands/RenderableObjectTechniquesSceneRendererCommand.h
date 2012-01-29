#pragma once

#ifndef _RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H
#define _RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;

class CRenderableObjectTechniquesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );

private:
	CPoolRenderableObjectTechnique		*m_pPoolRenderableObjectTechnique;
};

#endif