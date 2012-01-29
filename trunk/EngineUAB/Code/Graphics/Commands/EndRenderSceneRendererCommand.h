#pragma once

#ifndef _END_SCENE_RENDERER_COMMAND_H
#define _END_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CEndSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEndSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif