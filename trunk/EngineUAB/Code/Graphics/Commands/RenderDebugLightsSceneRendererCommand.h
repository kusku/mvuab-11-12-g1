#pragma once

#ifndef _RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectsManager;

class CRenderDebugLightsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif