#pragma once

#ifndef _RENDER_DEBUG_MODIFIERS_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_MODIFIERS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderDebugModifiersSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugModifiersSceneRendererCommand (CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif