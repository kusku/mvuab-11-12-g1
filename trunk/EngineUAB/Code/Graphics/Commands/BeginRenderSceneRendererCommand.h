#pragma once

#ifndef _BEGIN_SCENE_RENDERER_COMMAND_H
#define _BEGIN_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CBeginSceneRendererCommand : public CSceneRendererCommand
{
public:
	CBeginSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );
};

#endif