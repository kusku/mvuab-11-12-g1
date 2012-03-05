#pragma once

#ifndef _RENDER_DEBUG_PROCESS_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_PROCESS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderDebugProcessSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugProcessSceneRendererCommand (CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif