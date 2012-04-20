#pragma once

#ifndef _RENDER_GUI_SCENE_RENDERER_COMMAND_H
#define _RENDER_GUI_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderGUISceneRendererCommand (CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif