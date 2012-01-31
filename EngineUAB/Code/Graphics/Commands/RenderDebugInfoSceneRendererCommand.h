#pragma once

#ifndef _RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderDebugInfoSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugInfoSceneRendererCommand (CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif