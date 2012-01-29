#pragma once

#ifndef _RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectsManager;

class CRenderDebugSceneSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );

private:
	CRenderableObjectsManager		*m_pRenderableObjectsManager;
};

#endif